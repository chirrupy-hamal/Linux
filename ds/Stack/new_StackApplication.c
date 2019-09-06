#include<string.h>

#include"new_SeqStack.h"

void BracketMatch(const char *str, int size)
{
  assert(str);
  SeqStack stack;  
  //int i = 0;
  char ch;//当前字符
  char LeftBracket;
  SeqStackInit(&stack);
  for(int i = 0; i < size; ++i)
  {
    ch = str[i];
    switch(ch)
    {
      case '(':
      case '{':
      case '[':
        StackPush(&stack, (DataType)ch);
        break;
      case ')':
      case '}':
      case ']':
        if(SeqStackEmpty(&stack))
        {
          printf("右括号多\n");
          SeqStackDestroy(&stack);
          return;
        }
        LeftBracket = (char)SeqStackTop(&stack);
        SeqStackPop(&stack);
        if(LeftBracket == '(' && ch != ')')
        {
          printf("不匹配\n");
          return;
        }
        if(LeftBracket == '{' && ch != '}')
        {
          printf("不匹配\n");
          return;
        }
        if(LeftBracket == '[' && ch != ']')
        {
          printf("不匹配\n");
          return;
        }
      default:
        break;
    }
  }
  if(!SeqStackEmpty(&stack))
  {
    printf("左括号多\n");
    return;
  }
  printf("正常匹配\n");
}

int main()
{
  char *str[] = 
  {
    "(())abc{[(])}",
    "(()))abc{[]}",
    "(()()abc{[]}",
    "(())abc{[]()}",
    "",
//    NULL
  };
  int i = 0;
  for(; i < 5; ++i)
  {
    BracketMatch(str[i], strlen(str[i]));//strlen()没有计算'\0'在内
  }
  return 0;
}
