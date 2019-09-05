
#include"SeqStack.h"

typedef enum 
{
  OPERAND,//操作数类型
  OPERATOR//操作符类型
} Type;

typedef enum
{
  ADD,
  SUB,
  MUL,
  DIV
} Operator;

typedef struct
{
  Type type;
  int operand;//操作数
  Operator operator;//操作符
} Element;

//逆波兰表达式
int ReversePolishExpression(Element expression[], int size)
{
  SeqStack stack;
  int i = 0;
  Element e;//当前元素
  int op1 = 0;//操作数1
  int op2 = 0;//操作数2
  int result = 0;//最终结果
  SeqStackInit(&stack);
  for(; i < size; ++i)
  {
    e = expression[i];
    switch(e.type)
    {
      case OPERAND:
        StackPush(&stack, e.operand);
        break;
      case OPERATOR:
        switch(e.operator)
        {
          case ADD:
            //因为加法是二元操作符，所以从栈中取出两个数
            op1 = SeqStackTop(&stack);
            SeqStackPop(&stack);
            op2 = SeqStackTop(&stack);
            SeqStackPop(&stack);
            result = op1 + op2;
            StackPush(&stack, result);//结果也需要压栈，
                                      //以免下次运算
            break;
          case SUB:
          case MUL:
          case DIV:
            break;
          default:
            assert(0);//就不应该到这里来
        }
    }
  }
    assert(SeqStackSize(&stack) == 1);//到最后栈中只有一个数
    result = SeqStackTop(&stack);
    return result;
}

int main()
{
  //1+2+3
  //其逆波兰表达式为：1 2 + 3 +
  //不用担心优先级的问题，因为你最终都得把原有的表达式写成其对应的逆波兰表达式，
  //例如：
  //2-3*4+5        -> 2 3 4 * - 5 +
  //12*(3+4)-6+8/2 -> 12 3 4 + * 6 - 8 2 / +     
  Element expression[] = 
  {
    {
      OPERAND,
      1,
      SUB//这个其实没有用，四个随便写，只是占个地方而已，初始化的时候还必须得写出来
    },
    {
      OPERAND,
      2,
      SUB//这个其实没有用，四个随便写，只是占个地方而已，初始化的时候还必须得写出来
    },
    {
      OPERATOR,
      -1,//这个其实没有用，四个随便写，只是占个地方而已，初始化的时候还必须得写出来
      ADD 
    },
    {
      OPERAND,
      3,
      SUB//这个其实没有用，四个随便写，只是占个地方而已，初始化的时候还必须得写出来 
    },
    {
      OPERATOR,
      -1,//这个其实没有用，四个随便写，只是占个地方而已，初始化的时候还必须得写出来
      ADD
    }
  };
  int result = ReversePolishExpression(expression, 5);
  printf("%d\n", result);
}
