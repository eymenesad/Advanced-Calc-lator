#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_EXPR_SIZE 256


typedef enum {
    TOKEN_TYPE_NULLL,
    TOKEN_TYPE_NUMBER,
    TOKEN_TYPE_OPERATOR,
    TOKEN_TYPE_FUNCTION,
    TOKEN_TYPE_VARIABLE,
    TOKEN_TYPE_PARENTHESES,
    TOKEN_TYPE_COMMA
} TokenType;


typedef struct {
    TokenType type;
    char value[64];
} Token;

Token checkerToken[257];
Token arrToken[256+1];


char lookup[128][64];
int lookup_2[128];
int a=0;
int b=0;

int precedence(char operator)
{
    switch (operator) {
    case '+':
    case '-':
        return 1;
    case '*':
        return 2;
    case '&':
    case '|':
        return 0;
    
    default:
        return -1;
    }
}
 
int isOperator(char ch)
{
    return (ch == '+' || ch == '-' || ch == '*' || ch == '&' || ch == '|');
}

int infixToPostfix(char* infix)
{
    a=0;
    
    for(int i=0;i<257;i++){
        arrToken[i].type = TOKEN_TYPE_NULLL;
        checkerToken[i].type = TOKEN_TYPE_NULLL;

        strcpy(arrToken[i].value,"");
        strcpy(checkerToken[i].value,"");

    }
    Token stackToken[256+1];
    Token funcStack[50];
    int funcCounter=0;

    int i=0, j=0;
    int len = strlen(infix);
    int top = -1;
    while(i<len-1){
        if (infix[i] == ' ' || infix[i] == '\t'){
            
            /*if(i>0 && i<len-1 && isalnum(infix[i-1]) == isalnum(infix[i+1])){
                bigErrorCheck=1;
                break;
            }*/
            i++;
            continue;
        }
        Token token;
        
        
        if (isdigit(infix[i])) {
            token.type = TOKEN_TYPE_NUMBER;
            int k = 0;
            while(isdigit(infix[i])){
                token.value[k]=infix[i];
                k++;
                i++;
            }
            token.value[k] = '\0';
            arrToken[a++] = token;
            checkerToken[b++] = token;
            
        }

        else if (isalpha(infix[i])) {
            
            int k = 0;
            while(isalpha(infix[i])){
                token.value[k]=infix[i];
                k++;
                i++;
            }
            token.value[k] = '\0';
            
            if(strcmp(token.value, "xor") == 0 || strcmp(token.value, "ls") == 0||strcmp(token.value, "rs") == 0||strcmp(token.value, "lr") == 0||strcmp(token.value, "rr") == 0||strcmp(token.value, "not") == 0 ){
                token.type = TOKEN_TYPE_FUNCTION;
                funcStack[funcCounter++] = token;
                
            }else{
                token.type = TOKEN_TYPE_VARIABLE;
                arrToken[a++] = token;
            }
            checkerToken[b++] = token;
        }

        else if(infix[i] == ','){
            token.type = TOKEN_TYPE_COMMA;
            token.value[0] = ',';
            checkerToken[b++] = token;
            if(strcmp(funcStack[funcCounter-1].value,"xor")==0){

                while (top > -1 && precedence(stackToken[top].value[0]) >= precedence('^') && stackToken[top].type != TOKEN_TYPE_PARENTHESES){
                    arrToken[a++] = stackToken[top--];
                }
                stackToken[++top].type = TOKEN_TYPE_OPERATOR;
                strcpy(stackToken[top].value,"^");                
            }else if(strcmp(funcStack[funcCounter-1].value,"ls")==0){

                while (top > -1 && precedence(stackToken[top].value[0]) >= -1 && stackToken[top].type != TOKEN_TYPE_PARENTHESES){
                    arrToken[a++] = stackToken[top--];
                }
                stackToken[++top].type = TOKEN_TYPE_OPERATOR;
                strcpy(stackToken[top].value,"<<");                
            }else if(strcmp(funcStack[funcCounter-1].value,"rs")==0){

                while (top > -1 && precedence(stackToken[top].value[0]) >= -1 && stackToken[top].type != TOKEN_TYPE_PARENTHESES){
                    arrToken[a++] = stackToken[top--];
                }
                stackToken[++top].type = TOKEN_TYPE_OPERATOR;
                strcpy(stackToken[top].value,">>");                
            }else if(strcmp(funcStack[funcCounter-1].value,"lr")==0){

                while (top > -1 && precedence(stackToken[top].value[0]) >= -1 && stackToken[top].type != TOKEN_TYPE_PARENTHESES){
                    arrToken[a++] = stackToken[top--];
                }
                stackToken[++top].type = TOKEN_TYPE_OPERATOR;
                strcpy(stackToken[top].value,"lr");                
            }else if(strcmp(funcStack[funcCounter-1].value,"rr")==0){

                while (top > -1 && precedence(stackToken[top].value[0]) >= -1 && stackToken[top].type != TOKEN_TYPE_PARENTHESES){
                    arrToken[a++] = stackToken[top--];
                }
                stackToken[++top].type = TOKEN_TYPE_OPERATOR;
                strcpy(stackToken[top].value,"rr");                
            }
            funcCounter--;
            i++;
        }
       
        // if the scanned character is '('
        // push it in the stack
        else if (infix[i] == '(') {
            token.type = TOKEN_TYPE_PARENTHESES;
            token.value[0]=infix[i];
            i++;
            token.value[i] = '\0';
            stackToken[++top]=token; 
            checkerToken[b++]=token;
            if(strcmp(funcStack[funcCounter-1].value,"not")==0){
                stackToken[++top].type = TOKEN_TYPE_OPERATOR;
                strcpy(stackToken[top].value,"~");
                funcCounter--;
            }
        }
       
        // if the scanned character is ')'
        // pop the stack and add it to the strcmp(stackToken[top].value[0], '(')!=0
        // output string until empty or '(' found
        else if (infix[i] == ')') {
            checkerToken[b++].type=TOKEN_TYPE_PARENTHESES;
            strcpy(checkerToken[b].value,")");
            while (top > -1 && stackToken[top].type != TOKEN_TYPE_PARENTHESES ){
                arrToken[a++] = stackToken[top--];
            }
            //dikkat!!!
            if (top > -1 && stackToken[top].type != TOKEN_TYPE_PARENTHESES )
                ;
                //return "Error!";
            else
                top--;
            i++;
        }
       
       
        else if (isOperator(infix[i])) {
            token.type = TOKEN_TYPE_OPERATOR;
            token.value[0] = infix[i];
            token.value[1] = '\0';
            checkerToken[b++] = token;
            while (top > -1
                   && precedence(stackToken[top].value[0])
                          >= precedence(infix[i]))
                arrToken[a++] = stackToken[top--];
            stackToken[++top] = token;
            i++;
        }
    }
 
    while (top > -1) {
        if (stackToken[top].type == TOKEN_TYPE_PARENTHESES) {
            ;//return "Error!";
        }
        arrToken[a++] = stackToken[top--];
    }
    return 0;
}

int checkFunc(){
    checkerToken;
    for(int k=0;k<b-1;k++){
        if(checkerToken[k].type == TOKEN_TYPE_NUMBER){
            if(checkerToken[k+1].type == TOKEN_TYPE_NUMBER){
                return 1;
            }else if(checkerToken[k+1].type == TOKEN_TYPE_VARIABLE){
                return 1;
            }
        }
        if(checkerToken[k].type == TOKEN_TYPE_VARIABLE){
            if(checkerToken[k+1].type == TOKEN_TYPE_NUMBER){
                return 1;
            }else if(checkerToken[k+1].type == TOKEN_TYPE_VARIABLE){
                return 1;
            }
        }
        if(checkerToken[k].type == TOKEN_TYPE_OPERATOR){
            if(checkerToken[k+1].type == TOKEN_TYPE_OPERATOR){
                return 1;
            }
        }
        if(checkerToken[k].type == TOKEN_TYPE_FUNCTION){
            if(checkerToken[k+1].value[0] != '('){
            //if(checkerToken[k+1].type != TOKEN_TYPE_PARENTHESES){
                return 1;
            }
        }
        
    }
    return 0;
}

 
int evaluatePostfix()
{
    
    int toptstack=0;
 
    Token tstack[256];
    for(int l=0;l<257;l++){
        tstack[l].type = TOKEN_TYPE_NULLL;
        strcpy(tstack[l].value,"");
    }
 
    for (int i = 0; i< a; i++)
    {
               
        if(arrToken[i].type == TOKEN_TYPE_OPERATOR){
            char op_val[64];
            strcpy(op_val, arrToken[i].value);
            Token tok1 = tstack[--toptstack];
            int val1=0;
            if (tok1.type == TOKEN_TYPE_NUMBER){
                
                sscanf(tok1.value,"%d",&val1);
            }

            int outcome=0;
            if(strcmp(op_val,"~")==0){
                outcome = ~val1;
            }else{
                Token tok2 = tstack[--toptstack];
                int val2=0;
                if (tok2.type == TOKEN_TYPE_NUMBER){
                    sscanf(tok2.value,"%d",&val2);
                }
                
                
                
                if(strcmp(op_val,"+")==0){
                    outcome = val1+val2;
                }else if(strcmp(op_val,"-")==0){
                    outcome = val2-val1;
                }else if(strcmp(op_val,"*")==0){
                    outcome = val1*val2;
                }else if(strcmp(op_val,"&")==0){
                    outcome = val1&val2;
                }else if(strcmp(op_val,"|")==0){
                    outcome = val1|val2;
                }else if(strcmp(op_val,"^")==0){
                    outcome = val1^val2;
                }else if(strcmp(op_val,">>")==0){
                    outcome = val2>>val1;
                }else if(strcmp(op_val,"<<")==0){
                    outcome = val2<<val1;
                }else if(strcmp(op_val,"lr")==0){
                    outcome = (val2 << val1%32) | (val2 >> (32-val1)%32);
                }else if(strcmp(op_val,"rr")==0){
                    outcome = (val2 >> val1%32) | (val2 << (32-val1) % 32);
                }
            }
            
            tstack[toptstack].type=TOKEN_TYPE_NUMBER ;
            sprintf(tstack[toptstack].value,"%d",outcome);
            toptstack++;
        }else if(arrToken[i].type == TOKEN_TYPE_NUMBER){
            tstack[toptstack].type = arrToken[i].type;
            
            strcpy(tstack[toptstack].value,arrToken[i].value);
            toptstack++;
        }else if(arrToken[i].type == TOKEN_TYPE_VARIABLE){
            int val_var=0;
            for(int m=0;m<128;m++){
                if(strcmp(lookup[m], "")!=0 && strcmp(lookup[m], arrToken[i].value)==0){
                    val_var = lookup_2[m];
                    break;
                }
            }
            tstack[toptstack].type = TOKEN_TYPE_NUMBER;
            sprintf(tstack[toptstack].value,"%d",val_var);
            toptstack++;
        }
                
    }
    int res=0;
    sscanf(tstack[toptstack-1].value,"%d",&res);
    return res;
}
int isInsideLookup(char arr[]){
    int ind_look=0;
    for(int m=0;m<128;m++){
        if(strcmp(lookup[m], "")!=0 && strcmp(lookup[m], arr)==0){
            return ind_look;
            break;
        }
    }
    return -1;
}
int main()
{
    for(int k=0;k<128;k++){
        strcpy(lookup[k],"");
        lookup_2[k]=0;
    }

    int index_of_lookup=0;
    char line[256 +1] = "";
    printf("> ");
    while(fgets(line,sizeof(line),stdin)){
        
        if(line==NULL){
            break;
        }
        char line2[256+1]="";

        if(strchr(line,'%') != NULL){
            int ii=0;
            while(line[ii]!='%'){
                line2[ii]=line[ii];
                ii++;
            }
            line2[ii] = '\n';
            memset(line,'\0',sizeof(line));
            //strcpy(line,"");
            strcpy(line,line2);
        }
        int spaceFlag=1;
        for(int j=0;line[j]!='\n';j++){
            if(!isspace(line[j])){
                spaceFlag=0;
            }
        }
        if(spaceFlag==1){
            printf("> ");
            continue;
        }
        
        if(strchr(line,'=') == NULL){

            infixToPostfix(line);
            /*for(int jj=0;jj<b;jj++){
                
                printf(checkerToken[jj].value );

            }*/
            int check_number = checkFunc();
            if(check_number==1){
                printf("Error!\n");
                printf("> ");

                continue;
            }

            int ans = 0;
            ans = evaluatePostfix();
            printf("%d\n",ans);
            
        }
        else{
            char temp[257];
            strcpy(temp,line);

            
            char* token = strtok(temp, "=");
           
            char* value = strtok(NULL, "=");
            char token_array[64] = "";
            
            
            int hasError = 0;
            int my_flag=0;
            int i=0;
            while(token[i]!='\0'){
                
                if (isspace(token[i])) {
                    // skip whitespace characters
                    
                    i++;
                    continue;
                }
                
                if (!isalpha(token[i])){
                    hasError=1;
                    i++;
                }else {
                    if (my_flag == 1){
                        hasError=1;
                    }
                    int k = 0;
                    my_flag=1;
                    while(isalpha(token[i])){
                        token_array[k]=token[i];
                        k++;
                        i++;
                        
                    }
                    token_array[k] = '\0';
                }                 
                
            }
            if(hasError == 1){
                printf("Error!\n");
                printf("> ");
                continue;
            }
            
            int ifInside = isInsideLookup(token_array);
            
            
            infixToPostfix(value);
            int check_number = checkFunc();
            if(check_number==1){
                printf("Error!\n");
                printf("> ");
                continue;
            }
            int ans=0;
            ans = evaluatePostfix();


            if(ifInside==-1){
                strcpy(lookup[index_of_lookup],token_array);
                lookup_2[index_of_lookup] = ans;
                           
                index_of_lookup++;
            }else{
                lookup_2[ifInside] = ans;
            }
                                    
        }    
        printf("> ");
    }
    
    return 0;
}