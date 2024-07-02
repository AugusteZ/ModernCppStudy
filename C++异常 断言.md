# 异常 try catch

C++异常是对程序运行过程中发生的异常情况的**一种响应**。主要目的是设计容错程序提供语言级支持。

> 使在程序设计中包含错误处理功能更容易，以免事后采取一些更严格的错误处理方式。

异常提供了将控制权从程序的一个部分传递到另一个部分的途径。

对异常的处理有3个组成部分：

- 引发异常
- 使用处理程序捕获异常
- 使用`try`块



## 异常机制 try throw catch

C++异常是对程序运行过程中发生的异常情况的**一种响应**。

异常提供了将控制权从程序的一个部分传递到另一个部分的途径。

**`try` 块**，标识其中特定的异常可能被激活的代码块，它后面跟一个或多个`catch`块。

- 如果没有`throw`引发异常，会跳过所有的`catch`块。
- 引发异常：栈解退

**引发异常 throw**

- 分类

  - 非人为预设的`throw` 出错，编译器决定错误类型。

  ```cpp
  std::optional<int> a;
  try {
      int value = a.value();  // 抛出std::bad_optional_access
  } catch (const std::bad_optional_access& e) {
      std::cout << e.what() << '\n';
  }
  ```

  - **`throw`语句**，表示引发异常：将控制权向上返回到第一个，包含能够捕获相应异常的`try-catch`组合 的函数。

    返回的异常类型，是指定类型

    `throw bad_mean(a,b);`  调用`bad_mean`类的构造函数，类型`bad_mean`。

- 将终止函数的执行，程序沿函数调用序列后退，

  - 直到找到包含`try`块的函数。（栈解退）

  - 没有`try`块 或 没有匹配的catch处理程序时

    默认情况下，将调用`std::abort()`函数

**`catch` 捕获异常**。

- 异常 与 该处理程序匹配时，程序将指向`{}`中的代码块，指出要采取的措施。

- 异常类型

  - `catch(...){}`捕获任何异常。

  - 除此之外，位于括号中的类型声明，就是要响应的**异常类型**。

    - 通常是**类类型**。表明该处理程序 与 throw 的类型匹配。

    - 引发异常的函数将传递一个**对象**。异常触发时，会将参数赋给该参数。

      - **携带信息**：不同的类，存储不同的异常信息。

      - **代表异常**：作为类类型，代表不同的异常。

        使用不同的异常类型，区分不同的函数在不同情况下引发的异常。

```cpp
try { myfunc(); }
catch (bad_hmean & bg){
	bg.mesg(); // 使用对象的成员函数
} // 异常1 对应的类类型是 bad_hmean
catch (bad_gmean & hg){
    //...
} // 异常2 对应的类类型是 bad_gmean
catch (...) {} // 捕获所有其他的异常
```







## 栈解退

栈解退 `unwinding the stack`。

#### 异常终止

函数由于**出现异常**而终止，`try`块直接调用/间接调用 引发异常的函数

- 程序将释放栈中的内存，继续释放栈。直到找到一个位于`try`块中的返回地址。
- 随后，控制权将转到`try`块尾的**异常处理程序**。

这个处理  `try`块 和 `throw`之间整个函数调用序列放在栈中的对象 的过程称为**栈解退**。 

- 栈中的对象
  - 自动存储型变量——释放
  - 类对象——**调用析构函数**。
- 放在堆中对象，其**析构函数将不会调用**。



#### 正常调用

C++如何处理函数调用和返回的？

**函数调用**时，C++将信息放在栈中处理函数调用。

> 程序将调用**函数的指令的地址**（**返回地址**）到栈中。
>
> 当被调用的函数执行完毕后，程序将使用该地址来确定从哪里开始**继续执行**。（程序计数器PC）
>
> 函数参数被放到栈中，这些参数被视为自动变量。（被调用的函数创建的新的自动变量，也会被放入）。
>
> **被调用的函数A调用了另一个函数Aa，则后者Aa的信息将被添加到栈中。依次类推。**
>
> **函数结束**，程序流程将**跳回**到，该函数被调用时**存储的地址位置（在栈中的返回地址）**。同时，**释放**其自动变量。如果自动变量是，**类对象**，调用其**析构函数**（如果有的话）。
>
> 在释放栈的第一个返回地址后停止。如果函数被嵌套调用的话，则以此类推。



## 异常时

为了保证所有的系统都以相同受控方式运行未定义的代码，需要使用**异常**这一技术。

##### std::abort()

```cpp
#include <cstdlib>
#include <stdlib.h>
```

典型实现：

- 向标准错误流(`cerr`使用的错误流)发送消息

- 程序异常终止`abnormal program termination`，

- 返回一个随实现而异的值，告知 父进程/操作系统，处理失败。

是否刷新文件缓冲区（用于存储读写到文件中的数据的内存区域），取决于实现。

- 可以使用`exit()`函数，刷新文件缓冲区，但不显示消息。



##### 返回错误码

- 使用函数的返回值 指出问题，比异常更加灵活。

例如：`ostream`类的`get(void)`成员通常返回下一个输入字符的`ASCII`码，但到达文件尾时，返回特殊值`EOF`。

- 但有时，任何数值都是有效的返回值，可能**不存在**可以用于指出问题的**特殊值**。

  可使用**指针参数或引用参数**来**将值返回**给调用程序，并使用函数的返回值来指出成功还是失败。

> `istream`族，重载`>>`运算符使用了这种技术的变体。

通过告知调用程序 成功/失败，程序可以采取对应措施。



##### 全局变量

在某个地方存储返回条件的其它方法

- 使用一个**全局变量**。

可能出现问题的函数，可以在出现问题时，将该全局变量设置为**特定的值**，而调用程序可以检查该变量。

传统C语言数学库使用的就是这种方法，它使用的全局变量名为`errno`。

（需要确保其他函数没有将该全局变量用于其他目的）





# 异常规范 noexcept

[详解C++特性之noexcept (C++11 C++17 C++20)_cpp 移动构造函数后面noexcept-CSDN博客](https://blog.csdn.net/KingOfMyHeart/article/details/131882540)



## 目的

有时候，我们希望知道得知一个建议

——当前函数是否会抛出异常。

- 不关心抛出什么样类型的异常。

  在C++11前，使用`throw(optional_type_list)`来声明某些函数

- 这个建议是有用的。

我保证 自己写的这个 函数运行过程中不会抛出异常，所以：

- 给 接口调用者，作为使用者 不必为该接口写异常处理代码
- 给 编译器，可以更放心的做一些优化;

我自己的判断依据：按寻思何时使用noexept

1. 函数为一定不抛异常的函数
2. 即使有可能，默认出现异常时程序中止是最好的选择；



## noexcept

C++11中，既是 说明符，也是 运算符。

- 说明符`noexcept`关键字，指出函数不会引发异常。
- 运算符`noexcept()`，判断其他操作数是否会引发异常。

## 说明符

**作为异常说明符：**

我承诺，这个函数不会引发异常，你信我。

如果说，我水平有限最后又真的抛出了异常。

- 直接调用`terminate()` 函数结束进程，
- 狡辩到：该符号只是一种指示符号，不是承诺。
  - 是函数就有可能抛出异常，不能怪我，那就结束进程吧。
  - 或者，默认出现异常时的程序中止 就是我认为最好的做法，一切都符合预期；

```cpp
double marm() noexcept;  // marm()函数不会引发异常

double marm() noexcept(false);  // 显式标注marm()函数可能会引发异常
```





## 运算符

### 是什么

**作为运算符：**接受一个表达式，返回bool值。

- 如果编译器认为不会抛出异常，当表达式返回true

- 传入的表达式依赖编译器能在编译期找出表达式的可能的异常

  要在编译时判断，因此

  - 表达式必须是一个常量表达式

  - 不求值表达式，即不会执行表达式。

```cpp
void f1()noexcept{}

int* f2(int size)
{
    return new int[size];
}

int main()
{   
    std::cout << std::boolalpha;
    //声明了noexept ，说明不会抛出异常，返回true
    std::cout << noexcept(f1()) << std::endl;]
    //函数未声明noexept ，说明可能抛出异常，返回false
    std::cout << noexcept(f2(1000000)) << std::endl;
    return 0;
}
```



### 判断标准

**显式声明noexcept的函数**

```cpp
void f1()noexcept{}

class X
{
public:
    void fx()noexcept {}
    int GetValue()const noexcept { return v; }
private:
    int v = 100;
};
```

**默认带有noexcept声明的函数**

> 当对应类型的函数在基类和成员中具有noexcept声明：

1. 默认构造
2. 默认拷贝构造
3. 默认赋值函数
4. 默认移动构造
5. 默认移动赋值函数



## 使用

#### 标记函数noexcept

判断当前函数是不是 noexcept 的函数。

- 自己制定的标准，例如 ： type trait

```cpp
// 假设我们认为: 拷贝函数中，
// - 如果T是一个普通的编译器内置类型，那么该函数永远不会抛出异常，可以直接使用，
// - 当然，这里忽略了其它安全的类型
template <typename T>
T copy(const T& s) noexcept(std::is_fundamental<T>::value)
{
    //...
}
```

- noexcept()运算符的”传染“

  先判断某函数（T(s) 拷贝构造函数）是否会抛异常；

  - 不会抛出异常，则返回true
  - 可能会，则返回 false

```cpp
template <typename T>
T copy(const T& s) noexcept(noexcept(T(s)))
{
    //...
}
```



#### 重载决议

根据是否安全，使用不同的重载。

先判断有没有抛出异常的可能，

> noexcept()可以判断目标类型的移动构造函数是否可能抛出异常

- 有—— 使用传统的复制操作
- 没有—— 使用移动构造。

```cpp
template <typename T>
T swap_imp(T& a, T& b, std::integral_constant<bool ,true>) noexcept
{
    //...
}
template <typename T>
T swap_imp(T& a, T& b, std::integral_constant<bool, false>) 
{
    T tmp(a);
    a = b;
    b = tmp;
}

template <typename T>
T swap(T& a, T& b) 
    noexcept(noexcept(swap_imp(a,b, std::integral_constant<bool, noexcept(T(std::move(a))) && noexcept(a.operator=(std::move(b)))>())))
{
    swap_impl(a,b, std::integral_constant<bool, noexcept(T(std::move(a))) && noexcept(a.operator=(std::move(b)))>());
}

```





# 断言 assert



断言失败会显示错误信息并直接中断程序。

### 运行时 assert

运行时断言 assert：在运行到断言位置时才触发断言。

使用场景：

- 通常在 `Debug` 模式下使用而不在 `Release` 模式下使用

  （例如：头文件 `cassert` 通过宏 `NDEBUG` 对 `Debug` 和 `Release` 版本做了区分）

- 断言不能代替程序中的错误检查。

  - **通常用于检查参数或表达式的合法性，它只应该出现在需要断言表达式为 true 的位置，**

  - 如果表达式中涉及到外部输入，则不应该依赖断言。


```cpp
void resize_buffer(void* buffer, int new_size) {
    assert(buffer != nullptr);  // ok，检查参数合法性
    assert(new_size > 0);       // ok，检查参数合法性
    assert(new_size <= MAX_BUFFER_SIZE);  // ok，检查参数合法性
}

bool get_user_input(char c) {
    assert(c == '\0x0d');       // 不合适，断言不应该用于检查外部输入
}
```



### 静态断言 static_assert

[C++ 静态断言 static_assert - 知乎 (zhihu.com)](https://zhuanlan.zhihu.com/p/562366505)

对于断言表达式是常量表达式的情况，在编译期就进行检查，能帮助我们提早发现错误。

Cpp 11 `static_assert(expression, error_message);`

- 所有处理在编译期间执行，不允许有空间或时间上的运行时成本，**失败的断言会在编译阶段报错**。
- 可以在命名空间、类或代码块内使用。
- 断言失败可以显示丰富的错误诊断信息。

C++17 单参数静态断言 `static_assert(expression);`

```cpp
#include <type_traits>

class A {};
class B : public A { };
class C { };

template <class T>
class E {
    // 判断 T 是否继承自 A
    static_assert(std::is_base_of<A, T>::value, "T is not base of A");
};

int main() {
    // cpp11 双参数
    static_assert(sizeof(int) >= 4, "sizeof(int) >= 4");
    // cpp17 单参数
    static_assert(sizeof(int) >= 4);
    E<B> b;
    E<C> c;
}
```

