#include<stdio.h>
#include<stdlib.h>
#include<time.h>
typedef struct cards{    
    int num;
    int face;
}poker;

typedef struct stack{    //implement stack
    int top;
    int maxTop;
    poker **data; //store array of pointer of poker
}stack_t;

void iniStack(stack_t *st, int size){ //initiallize stack
    st->top = -1;
    st->maxTop = size - 1;
    st->data = (poker**)malloc(sizeof(poker*) * size); //specify size 
}
void iniPoker(poker *p, int number){ // initiallize poker
    p->num = number;
    p->face = 0;
}
int isEmpty(stack_t *st){
    if(st->top == -1) return 1;
    return 0;
}

int isFull(stack_t *st){
    if(st->top == st->maxTop) return 1;
    return 0;
}
int top(stack_t *st){
    return st->data[st->top]->num;
}
int push(stack_t *st, poker *card){
    if(isFull(st) == 0) {
        st->data[++st->top] = card;
        return 1;
    }
    return -1;
}

poker* pop(stack_t *st){
    if(isEmpty(st) == 0){
        st->data[st->top]->face = 1;
        return st->data[st->top--];
    }
    printf("The stack is Empty.\n");
}
int findKing(stack_t *st){ //find the king of the pile return index of poker array
    for(int i = st->top; i >= 0 && st->data[i]->face == 1; i--){
        if(st->data[i]->num % 13 == 12) return i;
    }
    return -1;
}
void printStack(stack_t *st){
    if(isEmpty(st) == 1)
        printf("The stack is empty.");
    else{
        for (int i=0; i <= st->top; i++){
            if(st->data[i]->face == 1){
                switch(st->data[i]->num/13){
                    case 0:
                        printf("黑桃%d ", st->data[i]->num%13+1);
                      break;
                    case 1:
                        printf("梅花%d ", st->data[i]->num%13+1);
                        break;
                    case 2:
                        printf("紅心%d ", st->data[i]->num%13+1);
                        break;
                    case 3:
                        printf("方塊%d ", st->data[i]->num%13+1);
                        break;
                }    
            }
            else{
                printf("*");
            }
        }
    }
    printf("\n");
}

void shuffle(poker *card, int size){ //reference "https://edisonx.pixnet.net/blog/post/54675090"
    int pos, temp;
    srand(time(NULL));
    for(int i = 0; i < size; i++) {
        pos = rand()%52;
        temp = card[i].num;
        card[i].num = card[pos].num;
        card[pos].num = temp;
    }    
}
void draw(stack_t *st1, stack_t *st2){ //push to stack1 from stack2(pop)
   if(isEmpty(st2) == 0)
       push(st1, pop(st2));
   if(isEmpty(st2) == 0) 
       st2->data[st2->top]->face = 1;
}
void showDeck(stack_t *pile, stack_t *wastePile, stack_t *outputPile){
    printf("Waste pile :");
    printStack(wastePile);
    for(int i = 0; i < 7; i++)
    {
        printf("Playing pile %d :", i+1);
        printStack(&pile[i]);
    }
    for(int i = 0; i < 4; i++)
    {
        printf("Output pile %d :", i+1);
        printStack(&outputPile[i]);
    }
    printf("\n"); 
}
int main(){
    stack_t pile[7], stock, wastePile, outputPile[4];
    for(int i = 0; i < 7; i++){         // initiallizing every stack(pile)
        iniStack(&pile[i], 13);
    }
    iniStack(&wastePile, 52);
    iniStack(&stock, 52);
    for(int i = 0; i < 4; i++){
        iniStack(&outputPile[i], 13);
    }
    srand(time(NULL));                  //changing random number table by time
    poker card[52];                     //declaring poker and initiallize it
    for(int i = 1; i <= 52; i++){
        card[i].num = i;
        card[i].face = 0;
    }
    shuffle(card, 52);
    int count = 0;
    for(int i = 0; i < 7; i++){         //distributing poker to pile1 to 7
        for(int j = 1; j <= i+1; j++){
            push(&pile[i], &card[count++]);
        }
        card[count-1].face = 1;
    }    
    while(count < 52){                  //rest of poker push into stock
        push(&stock, &card[count++]);
    }
    loop:while(isEmpty(&stock)==0){     //starting game
        draw(&wastePile, &stock);       //draw a card
        showDeck(pile, &wastePile, outputPile); 
        if(isEmpty(&wastePile))
            draw(&wastePile, &stock);
                                        //(a) 
        if(top(&wastePile)%13==0){      //if top of wastePile is axe, move to output pile 
            draw(&outputPile[top(&wastePile)/13], &wastePile);
            printf("Do (a)\n");
            goto loop;
        }
        for(int i = 0; i < 7 ; i++){    //if top of pile1 to 7 is axe, move to output pile
            if(isEmpty(&pile[i]) == 1) continue;
            if(top(&pile[i])%13==0){
                draw(&outputPile[top(&pile[i])/13], &pile[i]);
                printf("Do (a)\n");
                goto loop;
            }
        }
        for(int i = 0; i < 4; i++){     //if ouput pile isnt empty, find which top of pile 
            if(isEmpty(&outputPile[i]) == 1) continue; //can move to
            int num = top(&outputPile[i]);
            if(isEmpty(&wastePile) == 0 && num+1 == top(&wastePile)){
                draw(&outputPile[i], &wastePile);
                printf("Do (a)\n");
                goto loop;
            }
            for(int j = 0; j < 7; j++){
                if(isEmpty(&pile[j]) == 1) continue;
                if(num+1 == top(&pile[j])){
                    draw(&outputPile[i], &pile[j]);
                    printf("Do (a)\n");
                    goto loop;
                }
            }
        }
                                        //(b)
        for(int i = 0; i < 7; i++){     //move a poker from the top of waste pile to pile1~7
            if(isEmpty(&pile[i]) == 1) continue;//which it can move to
            int num1 = top(&wastePile), num2 = top(&pile[i]);
            if(num1%13+1 == num2%13){
                if(num1/13<2 && num2/13>1 || num1/13>1 && num2/13<2){
                    draw(&pile[i], &wastePile);
                    printf("Do (b)\n");
                    goto loop;
                }
            }
        }    
                                        //(c)
        for(int i = 0; i < 7; i++){     //find the pile which can move and place to the other 
            if(isEmpty(&pile[i]) == 1) continue;//pile match the rule
            for(int j = i+1; j < 7; j++){
                if(isEmpty(&pile[j]) == 1) continue;
                int num1 = top(&pile[i]), num2 = top(&pile[j]);
                if(num1%13 == num2%13-1){
                    if(num1/13<2 && num2/13>1 || num1/13>1 && num2/13<2){
                        draw(&pile[j], &pile[i]);
                        printf("Do (c)\n");
                        goto loop;
                    }
                }
            }             
        }
        //if any playing pile is empty, find the face-up king move to
        int empty = -1;                 //find the empty pile
        for(int i = 0; i < 7; i++){
            if(isEmpty(&pile[i]) == 1){ 
               empty = i;
               break;
            }
        }
        if(empty > -1){                 
            for(int i = 0; i < 7; i++){ //find king poker
                if(i == empty||isEmpty(&pile[i]) == 1) continue;
                int kingNum = findKing(&pile[i]);//kingNum is the index of king poker from pile
                if(kingNum == -1 || kingNum == 0) continue;//dont use the poker is bottommost of pile, because is meaningless 
                for(int j = kingNum; j <= pile[i].top; j++){//move king and above it to the other pile
                    push(&pile[empty], pile[i].data[j]);
                }
                pile[i].top = kingNum-1;        //modify top of pile
                pile[i].data[kingNum-1]->face = 1; //face up under the poker of king 
                printf("Move king of pile[%d] and above it to empty pile.\n", i);
                break;
            }
        }
        //(d)
        //(e)
        printf("Do nothing\n");
    }
    int win = 0;                        //caculate winning of game
    for(int i = 0; i < 4; i++){
        win += 5*(outputPile[i].top+1);
    }
    printf("You win $%d\n", win-52);
}
