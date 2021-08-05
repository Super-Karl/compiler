int func(int a,int b){
  return a + b;
}

void fun(int a[],int size){
  if(size > 5){
    a[4] = 5;
  }
}

void func2(int a,int b,int c,int d){
  a+b+c+d;
}

int main(){
  int a = 5;
  int b = a + 5;
  int c = a*5;
  func2(a,b,c,a);
  int d[6];
  d[1] = func(a,b);
  fun(d,6);
  while(a > 4){
    b++;
  }
  return 0;
}