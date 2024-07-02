# 关键字

## constexpr

constexpr是c++11新添加的特征，所有的 **`constexpr`** 变量都是 **`const`**。

目的是：**将运算尽量放在编译阶段，而不是运行阶段**。显式『要求』表达式编译时求值

constexpr可以修饰函数、结构体。

#### 对比const

字面意义：const是常量的意思，也就是后面不会发生改变，所以可以将计算的过程放在编译过程。

必须初始化该变量。

- **`const`** 变量的初始化可以推迟到运行时进行。 

- **`constexpr`** 变量必须在编译时进行初始化。

> 数组的大小必须是常量，在声明数组array时，用函数返回值，此时会报错
>
> 如果将constexpr修饰函数定义，则可以将函数返回值作为数组的大小。

```cpp
constexpr float x = 42.0;
constexpr float y{108};
constexpr float z = exp(5, 3);
constexpr int i; // Error! Not initialized
int j = 0;
constexpr int k = j + 1; //Error! j not a constant expression
```





#### **修饰函数**

可在编译时计算其返回值的函数。

- 使用代码需要编译时的返回值来初始化 **`constexpr`** 变量，

- 用于提供非类型模板自变量。 

当其自变量为 **`constexpr`** 值时，函数 **`constexpr`** 将生成编译时 constant（常数）。 使用非 **`constexpr`** 自变量调用时，或者编译时不需要其值时，它将与正则函数一样，在运行时生成一个值。 （此双重行为使你无需编写同一函数的 **`constexpr`** 和非 **`constexpr`** 版本。）

- 修饰的函数只能引用全局不变常量。
- 修饰的函数只能调用其他constexpr修饰的函数。

##### 限制

constexpr修饰的函数也有一定的限制：

- return语句

  - 函数体必须包括return 语句。

    尽量只包含一个return语句，多个可能会编译出错；

  - **`constexpr`** 函数必须只接受并返回文本类型，不能是 **`static`** 或线程本地的。

    它可能包含局部变量声明，但必须初始化该变量。 本地声明的变量不需要是 **`const`**，并且可以变化。

    文本类型是可在编译时确定其布局的类型。 以下均为文本类型：

    - void

    - 标量类型

    - 引用

    - Void、标量类型或引用的数组

    - 类：

      具有普通析构函数以及一个或多个 constexpr 构造函数且不移动或复制构造函数的类。 

      此外，其所有非静态数据成员和基类必须是文本类型且不可变。

- **`constexpr`** 函数可以是递归的。

- 函数体可以包含其他语句，但是不能是运行期语句，只能是编译期语句；

- 主体可以定义为 `= default` 或 `= delete`。

- 正文不能包含如何 **`goto`** 语句或 **`try`** 块。

- **`constexpr`** 非 **`static`** 成员函数不需要通过隐式方式 **`const`**。

- 不能是 **`static`** 或线程本地的。本地声明的变量不需要是 **`const`**，并且可以变化。

```cpp
// constexpr.cpp
// Compile with: cl /EHsc /W4 constexpr.cpp
#include <iostream>

using namespace std;

// Pass by value
constexpr float exp(float x, int n)
{
    return n == 0 ? 1 :
        n % 2 == 0 ? exp(x * x, n / 2) :
        exp(x * x, (n - 1) / 2) * x;
}

// Pass by reference
constexpr float exp2(const float& x, const int& n)
{
    return n == 0 ? 1 :
        n % 2 == 0 ? exp2(x * x, n / 2) :
        exp2(x * x, (n - 1) / 2) * x;
}

// Compile-time computation of array length
template<typename T, int N>
constexpr int length(const T(&)[N])
{
    return N;
}

// Recursive constexpr function
constexpr int fac(int n)
{
    return n == 1 ? 1 : n * fac(n - 1);
}

// User-defined type
class Foo
{
public:
    constexpr explicit Foo(int i) : _i(i) {}
    constexpr int GetValue() const
    {
        return _i;
    }
private:
    int _i;
};

int main()
{
    // foo is const:
    constexpr Foo foo(5);
    // foo = Foo(6); //Error!

    // Compile time:
    constexpr float x = exp(5, 3);
    constexpr float y { exp(2, 5) };
    constexpr int val = foo.GetValue();
    constexpr int f5 = fac(5);
    const int nums[] { 1, 2, 3, 4 };
    const int nums2[length(nums) * 2] { 1, 2, 3, 4, 5, 6, 7, 8 };

    // Run time:
    cout << "The value of foo is " << foo.GetValue() << endl;
}
```



## static_for

编译期循环





# 折叠表达式

## 递归 终止

在C++17之前，实现必须分成两个部分。终止条件要在递归条件之前。

```cpp
//[1] 终止条件
template<typename First>  
First foldSum1(First&& value)  
{  
    return value;  
}
//[2] 递归条件
template<typename First, typename... Rest>  
First foldSum1(First&& first, Rest&&... rest)  
{  
    return first + foldSum1(std::forward<Rest>(rest)...);  
}
 
auto i1 = foldSum1(58, 25, 128, -10);  //201
auto s1 = foldSum1(std::string("a "), std::string("b "), std::string("c"));//"a b c"
```





## 折叠表达式 17 `... op`

### 用途

需要谨慎使用，并考虑是否有其他更直观的方法可以达到相同的效果。

- 可以使代码更简洁、更易于阅读，

  但也可能会使代码更难以理解。

- 简化对C++11中引入的参数包的处理，避免某些情况下递归的使用。

```cpp
template<typename... T>
auto foldSum2(T... args)
{
    // 一元 无初值，不允许传递空参数包
    //  - 左折叠
    return (... + args); // ((arg1 + arg2) + arg3) ...
    //  - 右折叠
    return (args + ...); // ((arg3 + arg2) + arg1) ...
    // 二元 有初值，允许传递空参数包
	return (0 + ... + args); // 0 + ((arg1 + arg2) + arg3) ...
    
}
auto i2 = foldSum2(58, 25, 128, -10);  //201
auto s2 = foldSum2();//"a b c"
```



### 展开规则

折叠表达式共有四种语法形式：一元  二元，左折叠 右折叠 的两两组合。

- 一元 二元：`不允许/允许` 传递空参数包

- 左折叠 右折叠：
  - 初始值在 `左边/右边`，展开之后从`左边/右边`开始折叠——表达式求值顺序，从 `左/右边` 开始
  - 优先选 左折叠表达式

```shell
# 1.一元左折叠(unary left fold)
# - 展开前
( ... op pack )
# - 展开后
((arg_1 op arg_2) op ...) op arg_N
((1 + 2) + ...) + 9


# 2.二元左折叠(binary left fold)
# - 展开前
(init op ... op pack)
# - 展开后
(((arg_init op arg_1) op arg_2) op ...) op arg_N
(((0 + 1) + 2) + ...) + 9


# 3.一元右折叠(unary right fold)
# - 展开前
(pack op ...)
# - 展开后
arg_1 op (... op (arg_N-1 op arg_N))
1 + ( ... + (8 + 9))

# 4.二元右折叠(binary right fold)
# - 展开前
(pack op ... op init)
# - 展开后
arg_1 op (... op (arg_N−1 op (arg_N op arg_init)))
1 + ( ... + (8 + (9 + 0))
```

`op` 表示运算符

`pack`代表参数包：含有未展开的形参包。

`init`代表初始值：不含未展开的形参包

> `pack`和`init`不含优先级低于 转型表达式 的运算符的表达式。
>
> `()`开闭括号也是折叠表达式的一部分。



### 空包

将一元折叠用于长度为零的包展开时，只能使用下列运算符：

1) 逻辑与（&&）。空包的值是 true
2) 逻辑或（||）。空包的值是 false
3) 逗号运算符（,）。空包的值是 void()

```cpp
template<typename... Args>
bool all(Args... args) { return (... && args); }
template<typename... Args>
bool any(Args... args) {return  (... || args);}
 
bool b = all(true, true, true, false);
// 在 all() 中，一元左折叠展开成
//  return ((true && true) && true) && false;
// b 是 false
```





### 合法的 opt

二元运算符

- 合法的有32 个
- ``。

```shell
+ - * / % 
^ & | = < > << >> 
+= -= *= /= %= ^= &= |= <<= >>= == != <= >= 
&& || , .* ->*
```

所有的二元操作符

- `. `、` ->`、 `[]`
- `,`左叠表达式可以使用 逗号运算符

















# 重载决议

## concept

### concept约束

```cpp
// c++ concepts库内对整型的类型限制
template <typename T>
concept integral = std::is_integral<T>::value;

// 约束1使用约束2做逻辑运算
template <typename _Tp>
concept signed_integral = integral<_Tp> && is_signed_v<_Tp>;
```

```cpp
template <typename T> concept C = {};

template <C c>
struct M {};
// 要求TestClass满足concept要求
M<TestClass> m;
```



#### requires表达式

在编译期，检查模板实参（对某typename）是否满足指定的约束。 从而可以根据模板条件的不同，重定义函数。

```cpp
// 形式1
requires { requirement-seq }
// 形式2
requires ( parameter-list(optional) ) { requirement-seq }
```



#### `requirements-seq`

`requirements-seq` 可以是：简单要求、类型要求、复合要求、嵌套要求.

##### 简单要求

任意不以 `requires` 关键字开头的表达式，它断言该表达式是有效的。

> 只在语言层面上检查该表达式是否有效（编译通过即可），而不会对该表达式进行求值

```cpp
template <typename T>
concept Addable = requires(T a, T b) { a + b; };  // a + b 可通过编译即可
```

##### 类型要求

以`typename`关键字开始的要求

紧跟`typename`的是一个类型名，通常可以用来检查嵌套类型、类模板以及别名模板特化的有效性。

如果模板实参替换失败，则要求表达式的计算结果为`false`

概念`C`中有3个类型要求，分别为`T::inner、S<T>`和`Ref<T>`，它们各自对应的是对嵌套类型、类模板特化和别名模板特化的检查。

```cpp
template <typename T, typename T::type = 0>
struct S;
template <typename T>
using Ref = T&;

template <typename T> concept C = requires
{
    typename T::inner; // 要求嵌套类型
    typename S<T>; // 要求类模板特化
    typename Ref<T>; // 要求别名模板特化
};
```

##### 复合要求

```cpp
template <class T>
concept Check = requires(T a, T b) {
    { a.clear() } noexcept;  // 支持clear,且不抛异常
    { a + b } noexcept->std::same_as<int>;  // std::same_as<decltype((a + b)), int>
};
template <typename T>
concept C =
    requires(T x) {
    {*x};                                 // *x有意义
    { x + 1 } -> std::same_as<int>;       // x + 1有意义且std::same_as<decltype((x + 1)), int>，即x+1是int类型
    { x * 1 } -> std::convertible_to<T>;  // x * 1 有意义且std::convertible_to< decltype((x *1),T>，即x*1可转变为T类型
};
```

##### 嵌套要求

由若干条requires构成，每一条都需要满足。

```cpp
template <class T>
concept Check = requires(T a, T b) {
    requires std::same_as<decltype((a + b)), int>;
    //...
};
```



### concept使用

#### 模板函数

以下展示四种写法（关于requires表达式，稍后再讲）

```cpp
// 1.使用已定义的concept名 替换 typename
template <integral T>
T inc(T& a) { return ++a; }
```

```cpp
// 2.使用已定义的concept名 auto 替换 类型名
integral auto inc(integral auto a) { return ++a; }
```

```cpp
// reuires concepte名<T>
//  - 3.放在形参列表后面
template <typename T>
T inc(T a) requires integral<T> { return ++a; }

//  - 4.放在函数定义和template中间
template <typename T>
requires integral<T> 
T inc(T a) { return ++a; }

//    这种方法支持匿名concept
template <typename T>
requires (std::is_void_v<std::invoke_result_t<F>>)
T inc(T a) { return ++a; }
```











#### 表达式 `if constexpr`

查看对象是否包含某成员函数重载

```cpp
template <class T>
void gench(T t) {
    if constexpr (requires {t.mFunc1();}) {
        t.mFunc1();
    }
    // 可以判断某个函数的重载是否支持
    else if constexpr (requires {t.mFunc2();}) {
    	t.mFunc2();
    } else if constexpr (requires {t.mFunc2(1);}) {
    	t.mFunc2(1);
    } else if constexpr (requires {t.mFunc3(int);}) {
    	t.mFunc3(1);
    } else if constexpr (requires {t.mFunc4(std::declval<int>());}) {
    	t.mFunc4(1);
    } else {
        printf("nothing matched");
    }
}
```



## SFINAE

### enable_if

[C++11模板元编程—std::enable_if使用说明_std::enable_if_t-CSDN博客](https://blog.csdn.net/kpengk/article/details/119979733)

#### 用途

`std::enable_if` 基于类型特性条件性地从重载决议移除函数，并对不同类型特性提供分离的函数重载与特化的便利方法。

用在模板参数中，可以在C++20之前实现concept。

- 假如bool = false会退出当前的重载决议。
- 基于不同的条件，选择对应的类模板或函数模板：
  - 函数的返回值

  - 函数的参数

用在其它场景中，相当于在编译期进行检查 错误则编译失败。

- 假如bool = false会编译报错

#### 基本使用

`std::enable_if<bool, _Ty>`

```cpp
template <bool _Test, class _Ty = void>
struct enable_if {}; // no member "type" when !_Test
 
template <class _Ty>
struct enable_if<true, _Ty> { // type is _Ty for _Test
    using type = _Ty;
};
 
template <bool _Test, class _Ty = void>
using enable_if_t = typename enable_if<_Test, _Ty>::type;
```

第一个模板参数  `bool _Test`决定了是否改表达式是否是第二个参数的类型。

- 为 `true` 获取类型可以通过

  `enable_if<true, _Ty>`类型`_Ty` 特化为void （默认模板参数）

```cpp
// 获取类型
std::enable_if<true, int>::type // c++11   int
std::enable_if_t<true, std::string> // c++14 std::string
```

- 为 `false`

  - `enable_if::type` 不存在
  - 使用 `enable_if::type` 或`enable_if_v`会
    - 产生编译错误
    - 或重载决议的退出。

```cpp
std::enable_if_t<false, int> // 编译错误 / 重载决议的退出
```



#### REQUIRES宏

REQUIRES宏：封装`enable_if`

重载决议时，可以决定是重载还是跳出。判别标准：

- true 选择重载

  只要不报错，不管写的东西没什么意义

- false 跳出重载

  出现错误就跳出

```cpp
// cpp11
#define REQUIRES(x) typename std::enable_if<(x), int>::type = 0
// cpp14
#define REQUIRES(x) std::enable_if_t<(x), int> = 0
// cpp20 concept requires就好
```

参数 x，一般是`is_xx_v` `decltype` 或 `void_t` 通过编译是否成功来判断 



### 创造 bool 值

现在问题变成了在编译期，怎么根据条件的不同，获得对应的bool值。

#### 判断类型 `is_xxx_v `

is_xxx_v 判断后返回的 value。

```cpp
template <class F, REQUIRES(std::is_void_v<decltype(std::declval<F>()())>)>
auto invoke(F f) {
    std::cout << "F is void \n";
    f();
}

template <class F, REQUIRES(!std::is_void_v<decltype(std::declval<F>()())>)>
auto invoke(F f) {
    std::cout << "F is not void \n";
    return f();
}
```

参数 x，`is_xx_v` 

- x为 true，`std::enable_if ::type` 成功

  ```cpp
  template <class F, int paramInt = 0>
  auto invoke(F f) {
  ```

- x为 false，`std::enable_if ::type` 或 `std::enable_if_t` 失败。

  在模板参数外会编译失败，在模板参数中会导致退出当前的重载决议，去匹配其它重载。

  ```cpp
  template <class F, xxx = 0>
  auto invoke(F f) {
  ```





#### 结构体的成员变量

结构体特化后的成员变量`::bool value `。

替换 REQUIRES中的bool量 `x` 为 `std::void_t<xxx>` 。

##### 特化原理 void_t 

`std::void_t<xxx>`用在模板参数中

- 只有其中的表达式能够编译成功，才会特化为void
- 会导致退出当前重载决议，落回默认的特化。

```cpp
// 特定类型的特化 > 表达式的特化 > 默认特化

// 如果其它所有重载决议失败，落回到该默认特化
template <class T, class = void>
struct myStruct {
     static constexpr bool value = false;
};
// 根据表达式，尝试特化
template <class T>
struct myStruct<T, std::void_t<yourRxpr>> {
    // value
};
// 对特定单一类型进行特化
template <>
struct myStruct<MyClass2> {
    // value
};
```

```cpp
MyClass mClass{};
if (myStruct<MyClass>::value) {
    mClass.func1();
}
```



##### 定义结构体 void_t 

根据表达式是否通过编译而特化其中 value 的值。

```cpp
MyClass mClass{};
if (has_func1<MyClass>::value) {
    mClass.func1();
}
```

```cpp
// - 1. 检测 func1()重载决议失败，落回到该特化
template <class T, class = void>
struct has_func1 {
    static constexpr bool value = false;
};
// - 2. 尝试特化: 检测 func1()
template <class T>
struct has_func1<T, std::void_t<decltype(std::declval<T>().func1())>> {
    static constexpr bool value = true;
};

// 检测 func2(int, double)
// - 1. 落回到该特化
template <class T, class = void>
struct has_func2{
    static constexpr bool value = false;
};
// - 2. 尝试特化: 检测func2(int, double)
template <class T>
struct has_func2<T, std::void_t<
    decltype(std::declval<T>().func2(
                               std::declval<int>()
                               , std::declval<double>()
                           ))>> {
    static constexpr bool value = true;
};
```

##### 判断表达式

整个过程是：

1. 根据`void_t`特化上面的结构体。

   `has_func1<T>`

2. 从特化后的结构体取出bool成员变量和其它条件，作为表达式。

   `has_func1<T>::value && true ...`

3. 传表达式到 `REQUIRES宏` 的 enable_if 中，用于 `enable_if<(x), int>` 的特化。

   - true 同意重载决议
   - false 退出当前重载决议

   通过多个 / 没有任何重载决议通过，会编译报错。

```cpp
#define REQUIRES(x) typename std::enable_if<(x), int>::type = 0

// 有func1
template <class T, REQUIRES(has_func1<T>::value)>
auto invoke(T t) {
    std::cout << " has func1, not care func2 " << std::endl;
    t.func1();
}

// 没有func1 有func2
template <class T, REQUIRES(!has_func1<T>::value && has_func2<T>::value)>
auto invoke(T t) {
    std::cout << " no func1, has func2 " << std::endl;
    t.func2(1, 2);
}

// 没有func1 没有func2
template <class T, REQUIRES(!has_func1<T>::value && !has_func2<T>::value)>
auto invoke(T t) {
    std::cout << " no func1, no func2 " << std::endl;
}
```



