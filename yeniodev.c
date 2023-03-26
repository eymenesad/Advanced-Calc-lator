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

Token arrToken[256+1];


char lookup[128][64];
int lookup_2[128];
int a=0;

int precedence(char operator)
{
    switch (operator) {
    case '+':
    case '-':
        return 1;
    case '*':
    case '/':
        return 2;
    case '&':
    case '|':
        return 3;
    
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
        strcpy(arrToken[i].value,"");
    }
    Token stackToken[256+1];
    Token funcStack[50];
    int funcCounter=0;

    int i=0, j=0;
    int len = strlen(infix);
    int top = -1;
    while(i<len-1){
        if (infix[i] == ' ' || infix[i] == '\t'){
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
            
        }

        else if(infix[i] == ','){
            token.type = TOKEN_TYPE_COMMA;
            token.value[0] = ',';
            if(strcmp(funcStack[funcCounter].value,"xor")==0){

                while (top > -1 && precedence(stackToken[top].value[0]) >= precedence('^') && stackToken[top].type != TOKEN_TYPE_PARENTHESES){
                    arrToken[a++] = stackToken[top--];
                }
                stackToken[++top].type = TOKEN_TYPE_OPERATOR;
                strcpy(stackToken[top].value,"^");                
            }else if(strcmp(funcStack[funcCounter].value,"ls")==0){

                while (top > -1 && precedence(stackToken[top].value[0]) >= precedence('<<') && stackToken[top].type != TOKEN_TYPE_PARENTHESES){
                    arrToken[a++] = stackToken[top--];
                }
                stackToken[++top].type = TOKEN_TYPE_OPERATOR;
                strcpy(stackToken[top].value,"<<");                
            }else if(strcmp(funcStack[funcCounter].value,"rs")==0){

                while (top > -1 && precedence(stackToken[top].value[0]) >= precedence('>>') && stackToken[top].type != TOKEN_TYPE_PARENTHESES){
                    arrToken[a++] = stackToken[top--];
                }
                stackToken[++top].type = TOKEN_TYPE_OPERATOR;
                strcpy(stackToken[top].value,">>");                
            }/*if(strcmp(funcStack[--funcCounter].value,"lr")==0){

                while (top > -1 && precedence(stackToken[top].value[0]) >= precedence('^') && stackToken[top].type != TOKEN_TYPE_PARENTHESES){
                    arrToken[a++] = stackToken[top--];
                }
                stackToken[++top].type = TOKEN_TYPE_OPERATOR;
                strcpy(stackToken[top].value,"^");                
            }if(strcmp(funcStack[--funcCounter].value,"rr")==0){

                while (top > -1 && precedence(stackToken[top].value[0]) >= precedence('^') && stackToken[top].type != TOKEN_TYPE_PARENTHESES){
                    arrToken[a++] = stackToken[top--];
                }
                stackToken[++top].type = TOKEN_TYPE_OPERATOR;
                strcpy(stackToken[top].value,"^");                
            }if(strcmp(funcStack[--funcCounter].value,"not")==0){

                while (top > -1 && precedence(stackToken[top].value[0]) >= precedence('^') && stackToken[top].type != TOKEN_TYPE_PARENTHESES){
                    arrToken[a++] = stackToken[top--];
                }
                stackToken[++top].type = TOKEN_TYPE_OPERATOR;
                strcpy(stackToken[top].value,"^");                
            }*/
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
        }
       
        // if the scanned character is ')'
        // pop the stack and add it to the strcmp(stackToken[top].value[0], '(')!=0
        // output string until empty or '(' found
        else if (infix[i] == ')') {
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
            }/*else if (tok1.type == TOKEN_TYPE_VARIABLE){
                
                for(int m=0;m<128;m++){
                    if(strcmp(lookup[m], "")!=0 && strcmp(lookup[m], tok1.value)==0){
                        val1 = lookup_2[m];
                        break;
                    }
                }
            }*/
            Token tok2 = tstack[--toptstack];
            int val2=0;
            if (tok2.type == TOKEN_TYPE_NUMBER){
                sscanf(tok2.value,"%d",&val2);
            }/*else if (tok2.type == TOKEN_TYPE_VARIABLE){
                
                for(int m=0;m<128;m++){
                    if(strcmp(lookup[m], "")!=0 && strcmp(lookup[m], tok2.value)==0){
                        val2 = lookup_2[m];
                        break;
                    }
                }
            }*/
            
            int outcome;
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
                outcome = val1>>val2;
            }else if(strcmp(op_val,"<<")==0){
                outcome = val1<<val2;
            }
            /*switch (op_val)
            {
            case '+': 
                outcome = val1+val2;
                break;
            case '-': 
                outcome = val2-val1;
                break;
            case '*': 
                outcome = val1*val2;
                break;
            case '&': 
                outcome = val1&val2;
                break;
            case '|': 
                outcome = val1|val2;
                break;
            case '^': 
                outcome = val1^val2;
                break;
            }*/
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
        if(strchr(line,'=') == NULL){

            infixToPostfix(line);
            int ans = evaluatePostfix();
            printf("%d\n",ans);
        }
        else{
            char temp[257];
            strcpy(temp,line);

            char* token;
            token = strtok(temp, "=");
            
            for(int i=0;token[i]!='\0';i++){
                lookup[index_of_lookup][i] = token[i];
            }


            token = strtok(NULL, "=");
            infixToPostfix(token);
            int ans=0;
            ans = evaluatePostfix();
            lookup_2[index_of_lookup] = ans;
            
            printf("%s\n",lookup[index_of_lookup]);
            printf("%d\n",lookup_2[index_of_lookup]);
            
            index_of_lookup++;
                        
        }    
        printf("> ");
    }
    
    return 0;
}