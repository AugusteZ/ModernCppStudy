# 可调用类型

## 概念

可调用的类型（callable type）：函数名、函数指针、函数符（函数对象）、有名称的lambda 表达式。

可调用对象：

- 函数指针
- 具有operator()成员函数的类对象
  - 仿函数（函数符）
  - lambda表达式

- 可被转换为函数指针的类对象
- 类成员(函数)指针

- bind表达式或其它函数对象



 ```cpp
// lambda1.cpp -- use captured variables
#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <ctime>
const long Size = 390000L;

int main()
{
    using std::cout;
    std::vector<int> numbers(Size);

    std::srand(std::time(0));
    std::generate(numbers.begin(), numbers.end(), std::rand);
    cout << "Sample size = " << Size << '\n';
    
    
    /* 使用函数指针 */
    int count3 = std::count_if(numbers.begin(),numbers.end(),f3);
	int count13 = std::count_if(numbers.begin(),numbers.end(),f13);
	cout << "Count ... by 3 " << count3 
        << "\nCount ... by 13 " << count13;
    
    /* 使用函数符 */
    class f_mod{
    private:
        int dv;
    public:
        f_mod(int d=1) : dv(d) {}
        bool operator()(int x) { return (x % dv) == 0; }
    };
    count3 = std::count_if(numbers.begin(),numbers.end(),f3);
	count13 = std::count_if(numbers.begin(),numbers.end(),f13);
	cout << "Count ... by 3 " << count3
        << "\nCount ... by 13 " << count13;
    
    /* 使用lambda */
    // using lambdas
    count3 = std::count_if(numbers.begin(), numbers.end(), 
		      [](int x){return x % 3 == 0;});
    cout << "Count of numbers divisible by 3: " << count3 << '\n';
    count13 = 0;
    std::for_each(numbers.begin(), numbers.end(),
         [&count13](int x){count13 += x % 13 == 0;});
    cout << "Count of numbers divisible by 13: " << count13 << '\n';
	
    // using a single lambda
    count3 = count13 = 0;
    std::for_each(numbers.begin(), numbers.end(),
         [&](int x){count3 += x % 3 == 0; count13 += x % 13 == 0;});
    cout << "Count of numbers divisible by 3: " << count3 << '\n';
    cout << "Count of numbers divisible by 13: " << count13 << '\n';

    // std::cin.get();
    return 0;
}
 ```







## 函数指针

#### 概念

与数据项相似，函数也有地址。

函数的地址是 **存储其机器语言代码 的内存 的开始地址**。

对用户而言，不重要。但对程序而言，很有用。

> 例如：编写将另一个函数的地址 作为 参数 的函数。
>
> 这样第一个函数就能够找到第二个函数，并运行它。
>
> 与**直接调用**另一个函数相比，它允许在不同的时间传递不同函数的地址。意味着可以在不同的时间使用不同的函数。





#### 步骤

采用的机制：将程序员要使用的算法函数的地址传递给`estimate()`函数。

- 获取函数的地址
- 声明一个函数指针
- 使用函数指针来调用函数

##### 获取函数的地址

使用函数名(后面不跟参数)，就是函数的地址。

注意区分 函数的地址 和 函数的返回值。

> 例如 `myFunc()`函数，则`myFunc`就是该函数的地址。

将**函数**作为参数传递

> `process(myFunc);`
>
> `process()`函数调用使得`process()`函数能够在其内部调用`myFunc`函数。

将**函数返回值**作为参数传递

> `process(myFunc());`
>
> `process()`函数首先调用`myFunc`函数，然后将其**返回值**，传递给`process()`函数。



##### 声明函数指针

###### 要求

声明指向函数的指针时，必须指定 指针指向的函数类型。声明应该像函数原型那样指出有关函数的信息。

意味着，声明应指定

- 函数的**返回类型**
- 函数的**特征标**(参数列表)

声明指定 特定类型的函数的指针，可以首先编写这种函数的原型，然后用类似`(*pf)`替换函数名，这样`pf`就是这类函数的指针。

为提供正确的优先级，必须在声明中使用`()`将`*pf`括起。否则就不再是`(*pf)`一个指向函数的指针，而是`*pf()`意味着一个返回指针的函数。



###### 函数指针的表示

```cpp
// 以下4个函数原型的函数指针的表示是相同的，它们的特征标和返回类型相同。
const double * f1(const double ar[],int n);
const double * f1(const double [],int n);
const double * f1(const double *ar,int n);
const double * f1(const double *,int n);
// 但是函数定义必须提供标识符，只有以下两种形式可用。
const double * f1(const double ar[],int n){}
const double * f1(const double *ar,int n){}

// 相应的函数指针
const double * (*pa)(const double *,int);
// 可在声明的同时初始化 
const double * (*p1)(const double [],int) = f1;
// 单值初始化 可以使用auto
auto p2 = f1;

double av[3]={1.0,2.0,3.0};
// 返回值 类型 double* double类型的指针
cout << (*p1)(av,3) << p2(av,3);
// 返回 指针  指向的内容
cout << *(*p1)(av,3) << *p2(av,3);

```





##### 调用函数

正确声明后，便可以将相应函数的地址赋给它：

```cpp
// 函数原型
double pam(int); 
// 其正确的指针类型声明如下：
double (*pf)(int); 
// 指针pf 可以指向一个 参数是int类型，返回值是double类型的函数，
/* 它用(*pf)替换了 函数名pam
	pam是函数 (*pf)也是函数
	pf是函数指针
*/
pf = pam;
```

`(*pf)`扮演的角色与函数名相同，因此使用时，只需将它看作函数名即可。

```cpp
// 使用指针调用函数
double x = pam(4);
double y = (*pf)(4); // 虽然不好看，但是它提醒我们，代码正在使用函数指针。
// 实际上 C++也允许像使用函数名那样使用pf
double z = pf(4);

void estimate(int lines,double (*pf)(int));
estimate(50,pam);
estimate(50,(*pf));
```





##### 示例描述

举个例子，类似刷题网站leetcode等

设计`estimate()`函数，用来估算编写指定行数的代码所需的时间，并且希望不同的程序员都将使用该函数。

对于所有的用户来说，`estimate()`中一部分代码是相同的，但该函数允许每个程序员提供自己的算法来估计时间。

###### estimate()

```cpp
#include <iostream>
using namespace std;
double betsy(int);
double pam(int); 

void estimate(int lines,double (*pf)(int));

int main()
{
    int code;
    cout << "How many lines of code do you need?  ";
    cin >> code;
    cout << "Here's Betsy's estimate: \n";
    estimate(code,besty);
    cout << "Here's Pam's estimate: \n";
    double (*pf)(int);
    pf = pam;
    estimate(code,(*pf));
    return 0;
}

double betsy(int lns)
{
    return 0.05 * lns;
}
double pam(int lns)
{
    return 0.03 * lns；
}
void estimate(int lines,double (*pf)(int))
{
    cout << lines << " lines will take  ";
    cout << (*pf)(lines) << " hours \n";
}
```

#### 函数指针数组

##### 声明

**auto**

不能使用`auto`来初始化列表。自动类型推断只能用于单值初始化，不能用于初始化列表。

但是声明数组`pa`后，声明同类型的数组可以使用`auto`

```cpp
// 以下4个函数原型的函数指针的表示是相同的，它们的特征标和返回类型相同。
const double * f1(const double ar[],int n);
const double * f2(const double [],int n);
const double * f3(const double *ar,int n);
const double * f4(const double *,int n);
// 函数指针数组
const double * (*pa[4])(const double *,int n)={f1,f2,f3,f4};
/* auto 不能用于初始化列表。*/
// auto (*pa[4])(const double *,int n)={f1,f2,f3,f4};
// 但是声明数组pa后，声明同类型的数组可以使用`auto`
auto pb = pa;
```

##### 调用函数

数组名是指向第一个元素的指针。

```cpp
const double av[3] =  {1.0,2.0,3.0};
// 调用函数指针数组中 函数指针指向函数的 返回值
const double * px = pa[0](av,3);
const double * py = (*pa[1])(av,3);
// 获取指针的内容
double x = *pa[0](av,3);
double y = *(*pa[1])(av,3);
```

##### 函数指针数组的指针

上述的数组名`pa` 是指向 函数指针的指针，也就是 指向指针的指针。

指向函数指针数组的指针并不少见，类的虚方法实现通常都采用了这种技术。(这些细节由编译器处理)

###### auto

由于可使用单个值进行初始化，可使用`auto`

`auto pc = &pa;`

潜在的缺点：

> 自动类型推断确保变量的类型与赋给它的初值的类型一致，但是，我们提供的初值的类型，可能不对。

###### typedef

关键字typedef也可以简化代码

`typedef const double *(*p_fun)(const double *,int);`

上述代码，将`p_fun`定义为一个类型名。

> `p_fun`是一个函数指针，返回值为`const double *`，参数列表`(const double *,int)`

```cpp
// 函数指针数组
p_fun pa[3] = {f1,f2,f3};
const double *(*pa[3])(const double *,int) = {f1,f2,f3};
// 指向函数指针数组的指针
p_dun (*pd)[3] = &pa;
const double *((*pa)[3])(const double *,int) = {f1,f2,f3};
```



###### 数组名 数组地址

两者从数字上相同，但类型不同。

`int pa[5] = {1,2,3,4,5};`

- `pa`指向第一个元素的指针也就是`&pa[0]` 
- `&pa`整个数组的地址。

差别体现在，

- 加法运算时，二者跳过的字节数不同。
- 要得到第一个元素的值，`pa`需要解除一次引用，`&pa`需要解除两次引用

`**&pa == *pa == pa[0]`

###### 示例1

```cpp
// 含4个函数指针的 数组
*pa[4] // *表示 数组中装的类型是指针
const double * (*pa[4])(const double *,int n)={f1,f2,f3,f4};

// 指向4个元素数组的 指针
(*pd)[4] 
/* 
	*表示是它指针，指向数组的指针 
	 
	[]表示数组元素的个数 
*/
const double * (*(*pd)[4])(const double *,int n) = &pa;
auto pd = &pa;
```





#### 示例

使用一个指向标准函数`rand()`，编写的`f3()`和`f13()`的**函数指针**，作为**函数对象**。

随机生成一个整数列表，判断其中多少个可以被3整除、多少个可以被13整除。

- 生成列表：`vector<int>`存储数字。`vector<int> numbers(100);`

- STL算法`generate()`：参数1、2指定区间，参数3是不接受任何参数的**函数对象**。函数`generate()`设置所有元素为参数3的返回值。

  > 用于生成随机数`generate(numbers.begin(), numbers.end(), rand);`

- 算法`count_if`：参数1、2指定区间，参数3返回`bool`值的函数对象。函数`count_if`计算这样的元素数。

  > `bool f3 (int x) {return x % 3 == 0;}`
  >
  > `bool f13(int x) {return x % 13 == 0;}`

```cpp
vector<int> numbers(100);
srand(std::time(0));
std::generate(numbers.begin(), numbers.end(), rand);
int count3 = std::count_if(numbers.begin(),numbers.end(),f3);
int count13 = std::count_if(numbers.begin(),numbers.end(),f13);
cout << "Count ... by 3 " << count3
    << "\nCount ... by 13 " << count13;
```





## 仿函数/函数符

函数符，是一个重载了类方法`operator()()`的类对象。

优点之一：可以使用同一个函数符完成多个任务。

随机生成一个整数列表，判断其中多少个可以被3整除、多少个可以被13整除。

> 创建存储特定整数值的`f_mod`对象，可以使用方法`operator()()`返回一个`bool`值。
>
> 构造函数本身可以应用于`count_if`函数的参数。创建无名的、临时对象如`f_mod(3)`，`f_mod(13)`。

```cpp
class f_mod{
private:
    int dv;
public:
    f_mod(int d=1) : dv(d) {}
    bool operator()(int x) { return (x % dv) == 0; }
};
```



```cpp
f_mod obj(3);
bool is_div_by_3 = obj(7);
int count3 = count_if(numbers.begin(),numbers.end(),f_mod(3));
int count13= count_if(numbers.begin(),numbers.end(),f_mod(13));

```







## lambda

lambda一种定义与应用函数的数学系统，允许使用匿名函数 —— 无需给函数命名。

对于接受函数指针或函数符的函数，可使用匿名函数定义作为其参数。

- lambda 
  - `[] (int x) {return x % 3 == 0;}`
  - `[] (int x) -> bool {return x % 3 == 0;}`

- 对应的函数定义 
  - `bool f3(int x){return x % 3 == 0;}`




### 用途

主要目的，能够将类似于函数的表达式用作接受函数指针或函数符的函数的参数。

- **可自动推断返回类型**。简洁、易于理解。

- **动态变量**： lambda可访问作用域内的任何动态变量。

- **定义靠近引用**

  - lambda的定义和使用是在同一个地方进行的，让定义位于使用的地方附近

    - 无需翻阅多页的源代码，以了解使用的函数。

    - 如果需要修改代码，涉及的内容都将在附近。

  - 在函数内部定义其他函数，因此函数的定义可能离使用它的地方很远。



### 语法

```shell
[capture list] (paramters) mutable throw()
-> return-type { statement }
```



#### 捕获列表 (值/引用)

**取消函数名**。按照值、引用，使用`[]   [&]   [=]...`替代

 lambda可访问作用域内的任何动态变量。使用`[]`替代了函数名。

变量名

- 指定变量名，`[z]`按值访问变量`z`。
- 名称前加`&`，`[&z]`按引用访问变量`z`。

所有动态变量(自动变量)

- `[&]`按引用访问所有动态变量。
- `[=]`按值访问所有动态变量。

```cpp
int count3 = 0;
int count13= 0;
/* 这里[&]能在lambad表达式中使用所有的自动变量 */
std::for_each(numbers.begin(), numbers.end(), 
             [&] (int x){ count3+= x%3 == 0; count13+= x%13 == 0;})
```

混合使用

- `[a,&b]`，按值访问a，按引用访问b。
- `[&,a]`，  按值访问a，按引用访问所有其他动态变量。
- `[=,&a]`，按引用访问a，按值访问所有其他动态变量。

```cpp
int count3 
count3 = std::count_if(numbers.begin(), numbers.end(), 
                           [](int x){return x % 3 == 0;});
```



#### 可忽略三件套

##### 参数列表

可以省略

- 和普通函数的参数列表一致。
- 如果不需要参数传递，则可以连同括号“()”一起省略。



##### 可变规格

例如：

默认情况下Lambda函数总是一个const函数

- mutable修饰符， 可以取消其常量性。
- 在使用该修饰符时，参数列表不可省略（即使参数为空）。



##### 异常说明

用于Lamdba表达式内部函数抛出异常。



#### 返回类型

##### 无返回值

可以在不需要返回值的时候也可以连同符号 `->` 一起省略。



##### 有返回值

###### 自动推导(忽略)

返回类型。

- 返回值相当于使用`decltype`根据返回值推断得到的。

- 仅当lambda表达式完全由一条返回语句组成时，自动类型推断才管用。

  > 对于判断整数列表中多少个可以被3整除：

```cpp
int count3 = std::count_if(numbers.begin(), numbers.end(), 
[](int x){return x % 3 == 0;});

```



###### 显式声明 `->`

使用新增的返回类型后置语法。

```cpp
// 指明返回类型为`double`
[](double x) -> double {int y=x; return x-y};
```

追踪返回类型形式声明函数的返回类型。

我们此外，在返回类型明确的情况下，也可以省略该部分，让编译器对返回类型进行推导。



#### 函数体

lambda函数体内容与普通函数一样，不过除了可以使用参数之外，还可以使用所有捕获的变量。



### 执行时机

Lambda表达式工作原理：

编译器会把一个lambda表达式生成一个匿名类的匿名对象，并在类中重载函数调用运算符,实现了一个operator()方法。

```cpp
#include <cstdio>
#include <iostream>

int main()
{
    // mLambda1 没有立即执行，返回值是 lambda表达式
    auto mLambda1 = []{ 
        std::cout << "Hello World 1!" << std::endl; 
        return 1;
    };
 	std::cout << " ------------ " << std::endl;
    auto rt1 = mLambda1(); // rt1 是lambda执行的返回值 int(1)
    
    // 第二条lambda立即执行, rt2 是lambda执行的返回值 int(2)
  	auto rt2 = []{
        std::cout << "Hello World 2!" << std::endl;
        return 2;
    }(); // 注意这个 (), 调用了operarter()()
  
  	return 1;
}
```







### 有名称的lambda auto

```cpp
auto mod3 = [](int x){return x % 3 == 0;};
count1 = std::count_if(n1.begin(), n1.end(), mod3);
count2 = std::count_if(n2.begin(), n2.end(), mod3);
```











## std::bind

### 用途

std::bind将可调用对象与其参数绑定成一个仿函数，

- 支持延迟调用到任何我们需要的时候。

- 绑定后的结果可以使用std::function保存。

用途

- 将可调用对象与其 全部参数 一起绑定成一个仿函数。
- 将可调用对象与其 部分参数 一起绑定。将多元（参数个数为 n， n>1）可调用对象转成一元或者（ n-1）元可调用对象，即只绑定部分参数。减少可调用对象传入的参数。

一般使用 `auto` fr 保存 std::bind 的返回结果，是因为我们并不关心 std::bind 真正的返回类型。





### 返回类型

保存 std::bind 的返回结果

- `auto` ——不关心 std::bind 真正的返回类型

- `std::function` 
  - 实际上 std::bind 的返回类型是一个 stl 内部定义的仿函数类型，因此可以直接赋值给一个 std::function。
  - 必须让占位符保持对应的位置。1就一定是1

通过 std::function 和 std::bind 的配合，所有的可调用对象均有了统一的操作方法。

```cpp
auto mBind1 = std::bind(&mfunction, std::placeholders::_1, property, uid);


std::function<int(std::string, std::vector<int>, int)> mfunc =  std::bind(&do_print, "wangwu", std::placeholders::_2, std::placeholders::_3);


// 执行
int ret =0;
ret = mfunc("lisi", 123, 1);
ret = mBind1("lisi", 123, 1);
```



### 参数

#### 可执行对象

##### 函数/函数指针

普通函数

参数1：

- 函数指针
- 或 函数名，函数名会隐式转换成函数指针。

```cpp
#include <functional>
// ***************************
// 			普通函数
// ***************************
double callableFunc (double x, double y) {return x/y;}

auto NewCallable_1 = std::bind (callableFunc, std::placeholders::_1, 6);

auto NewCallable_2 = std::bind (&callableFunc, std::placeholders::_1, 8);

// 相当于 callableFunc(10, 6);
std::cout << NewCallable_1(10) << std::endl;
// 相当于 callableFunc(12, 8);
std::cout << NewCallable_2(12) << std::endl;
```



##### 成员函数

成员函数

参数1：

- 成员函数的指针（必须显式地指定指针类型）

参数2：

- 对象的指针（必须显式地指定指针类型）

```cpp
#include <functional>        
// ***************************
// 			成员函数
// ***************************
class Base{
public:
    void display_sum(int a1, int a2)
    {
        std::cout << a1 + a2 << '\n';
    }
 
    int m_data = 30;
};
int main() 
{
    Base base;
    auto newiFunc = std::bind(&Base::display_sum, &base, 100, std::placeholders::_1);
    newiFunc(20); // should out put 120. 
}
```



##### 成员变量

有点奇怪的用法

参数1：

- 成员变量的指针

参数2：

- 对象的指针

```cpp
class A
{
public:
    int i_ = 0;
}

std::function<int&(void)> fr_i = std::bind(&A::i_, &a);
fr_i() = 123;
std::cout << a.i_ << std::endl; // 输出 : 123
```





#### 占位符 placeholders

占位符：以调用时传入的参数为准

- 代表这个位置将在函数调用时，被传入的第x个参数所替代。
- 甚至可以“跳跃”

`std::placeholders::_1`  `_2`

```cpp
#include <functional>

int TestFunc(int a, char c, float f)
{
    std::cout << a << std::endl;
    std::cout << c << std::endl;
    std::cout << f << std::endl;
    return a;
}

// 使用占位符
auto fun1 = std::bind(TestFunc, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
auto fun2 = std::bind(TestFunc, std::placeholders::_2, std::placeholders::_3, std::placeholders::_1);
auto fun3 = std::bind(TestFunc, 30, std::placeholders::_2, 100.1);

// 以下使用均等价
fun1(30, 'C',100.1);
fun2(100.1, 30, 'C');
fun3(40,'C');		// 没有占位符 _1，40被忽略

```



#### 引用参数

默认情况下，bind的非占位符的参数，会被拷贝到bind返回的可调用对象中。

但是，与lambda类似，有时对有些绑定的参数希望以引用的方式传递，或是要绑定参数的类型无法拷贝。

```cpp
std::vector<int> property = {1, 2, 3};

auto mBind3 = std::bind(do_print, std::ref(property));

```



### 一点花里胡哨

```cpp
using std::placeholders::_1;

// 查找元素值大于 10 的元素的个数
int count = std::count_if(
        coll.begin(), coll.end(), 
        std::bind(less<int>(), 10, _1)
    );

// 查找元素之小于 10 的元素
int count = std::count_if(
    	coll.begin(), coll.end(),
    	std::bind(less<int>(), _1, 10)
	);


// 查找元素之小于 10 大于5 的元素
// 复合多个函数（或者说闭包）的功能
auto sbind1 = 
    std::bind(std::logical_and<bool>(),
        std::bind(std::greater<int>(), _1, 5),
        std::bind(std::less_equal<int>(), _1, 10)
    );
int ret = sbind1(6);
```



# std::function

类型擦除技术，使用到了虚函数。类似的还有 `std::any`。

std::function 一个可调用的对象封装器。在编码时可以通过指定模板参数使用统一的方式处理函数、函数指针。

主要有以下场景：

- 绑定一个函数（普通函数或者静态函数）
- 实现回调函数
- 作为函数入参

通过类进行实现

- 可以把std::function看做一个函数对象，用于表示函数这个抽象概念。

- std::function的实例可以存储、复制和调用任何可调用对象，
- 存储的可调用对象称为std::function的目标，
- 调用空的std::function的目标会抛出std::bad_function_call异常。





# 协程

## 概念

### 协程是什么

协程(Coroutine)，一种特殊的函数

- 在函数执行到某个地方的时候可以暂停执行，返回给调用者或恢复者（可以有一个返回值）
- 暂停的是什么？
  - 暂停执行协程函数本身
  - 不暂停协程函数所在的线程
- 允许从暂停的地方恢复继续执行

### 协程的意义

协程的概念在很早就出现了，甚至其他语言（JS，Python，C#等）早就已经支持了协程。

最常见的用途，就是将“异步”风格的编程“同步”化。同一个线程中

> 和多线程类似，但是避免了资源竞争，因为只有一个线程。

- 在一个时间点最多只能跑一个协程
- 协程的运行是穿行的，没有数据争用(data race)，也不需要锁。

```cpp
// 用同步调用的方式进行异步调用
task<void> request()
{
	int rt = co_await requestA(1);
	// 处理一些中间结果
	rt = co_await requestB(rt);
	// 处理一些中间结果
	rt = co_await requestC(rt);
	// 对最终结果做一些事情
}
```



### 怎么才算协程函数

C++20中，**协程是特殊函数，但不是由普通函数添加一个关键字组成的**

> 想要让某个函数成为协程函数，需要人工的做一些额外的工作，
>
> 一些辅助的自动化的工具应该会在C++23标准中提供，以后会更方便的使用。

- 使用了 任意关键字 `co_return`，`co_yield`，`co_await`

- 返回值 符合Promise规范的类
  - 类中定义  `struct promise_type`
  - 其中包含了符合规范的5个特定函数

```cpp
struct task{
  struct promise_type {
    auto get_return_object() { return task{}; }
    std::suspend_never initial_suspend() { return {}; }
    std::suspend_never final_suspend() { return {}; }
    void return_void() {}
    void unhandled_exception() {}
  };
}
task getTask() { // 协程函数
	// 实现中不能写return
	co_return;
}
```



## 运算符

### `co_await` 运算符

`co_await Awaitable();`

**用途**：按照同步的方式去调用异步函数

**使用方法**：后面跟随 Awaitable类 `“实现了三个特定函数的类”`。

```cpp
// suspend_always 就是一个 Awaitable类
struct suspend_always {
	constexpr bool await_ready() const noexcept { return false; }
	constexpr void await_suspend(std::coroutine_handle<> h) const noexcept {}
	constexpr void await_resume() const noexcept {}
};
```

使用 `co_await Awaitable();`

```cpp
co_await suspend_always();
```

1. 构造suspend_always

   调用构造函数：可通过构造函数模仿一个普通的函数调用

2. 判断是否需要等待？根据实现的`await_ready()`的返回值
   - true，不需要等待
   - false，需要等待

3. 需要等待

   - 执行`await_suspend` 

     可以通过传统的回调函数法执行一些异步操作

   - 暂停该协程的运行，不会影响所在线程的执行。

4. 不需要等待 / 从等待恢复

   - 执行 `await_resume`

   - 返回值：

     - co_wait的返回值就是 `await_resume`函数的返回值
     - await_resume函数允许拥有任意的返回值类型，模板类型也是允许的。

   - 恢复方法

     在回调函数中调用`std::coroutine_handle<>`的`resume函数`恢复。
