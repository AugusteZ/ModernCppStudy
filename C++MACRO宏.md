# 宏

神奇的用法



## 宏定义&取消`#define`  `#undef `

`#defined` `#undef `

如果要查看多个宏是否定义过，可使用下面的预处理指令

- `#define` 定义宏

```cpp
// 定义一个宏，其内容为空
//#if (MACRO) 报错
#define PXLW_IRIS
```

```cpp
// 定义一个宏，其内容不为空(允许 bool和int)
// #if (MACRO) 为 true
#define PXLW_IRIS_VAL_TRUE		7
#define PXLW_IRIS_BOOL_TRUE		true
// #if (MACRO) 为 false
#define PXLW_IRIS_VAL_FALSE		0
#define PXLW_IRIS_BOOL_FALSE	false
```

- `#undef` 取消宏

```cpp
#define MAX 200
printf("MAX = %d\n", MAX);
#undef MAX
```



## 逻辑控制

通过和预处理指令配合，达到一定的代码开关控制，常见的比如在跨平台开发时，对不同的操作系统启用不同的代码。

### `if else ifdef全家桶`

```cpp
#ifdef MACRO		// MACRO宏定义过则满足条件

#if (condition)
#elif (condition)
#else
#endif
```

`#if` 后面的 condition 是否为真了。

```cpp
#ifdef _WIN32 // 查看是否定义了该宏，Windows默认会定义该宏
    // 如果是Windows系统则会编译此段代码
    OutputDebugString("this is a Windows log");
#else
    // 如果是mac，则会编译此段代码
    NSLog(@"this is a mac log");
#endif
```





### condition

可以被if判断的condition只能是 整数 / bool量，定义为其他类型会编译错误。

```cpp
#define MACRO true
if (std::is_same<bool, decltype(MACRO)>::value) {
    std::cout << "MACRO can be bool" << std::endl;
} else if (std::is_same<int, decltype(MACRO)>::value) {
    std::cout << "MACRO bool is fake, trans to int" << std::endl;
} else {
    std::cout << "MACRO bool is fake, i dont know what it is" << std::endl;
}

# MACRO can be bool
```

#### 字面量

`#if` 为真

1. 非 0 整数：`-100`，`-1` ，`1`
2. 布尔量：`true`

为假

1. 整数 0：`0`
2. 布尔量：`false`

```cpp
// 真
#if true
#elif 1
// 假
#elif false
#elif 0
#endif
```



#### 简单的表达式

##### 整数运算

支持对整数进行的简单运算

 `>=`，`==`，`<=`，`<`，`>`

```cpp
#define PXLW_CHIP_VERSION 7

#if (PXLW_CHIP_VERSION > 7)
#elif (PXLW_CHIP_VERSION < 7)
#elif (PXLW_CHIP_VERSION == 7)
#endif
```



##### 与或非

| 含义 | 符号         |
| ---- | ------------ |
| 与   | `&&`         |
| 或   | `||`         |
| 非   | `!`    `not` |

```cpp
#if (condition_and_1) && (condition_and_2)
#elif (condition_or_1) || (condition_or_2)
#elif ! (condition_not_1)
#elif not (condition_not_2)
#else 
#endif
```

```cpp
#if defined(_WIN32) || defined(WIN32)
    // 如果是Windows系统则会编译此段代码
    OutputDebugString("this is a Windows log");
#elif !defined(WIN32) && !defined(__WATCOMC__) && \
	!defined(__VXWORKS__)
	OutputDebugString("this is unkown log");
#endif
```



#### 宏的内容

```text
#define MACRO 1

#if (MACRO)

#elif not (MACRO)

#elif ! (MACRO)

#endif
```



#### 宏的存在 `#ifdef`  `defined`

宏定义过就满足条件

- `#ifdef` 宏

```cpp
#ifdef _WIN32 // 查看是否定义了该宏，Windows默认会定义该宏
    // 如果是Windows系统则会编译此段代码
    OutputDebugString("this is a Windows log");
#else
    // 如果是mac，则会编译此段代码
    NSLog(@"this is a mac log");
#endif
```

- `defined` 判断是否定义过

```cpp
// 定义过
#if defined(IRIS_MTK_SPECIFIC)
#elif defined MACRO
// 没定义过
#elif not defined(IRIS_MTK_SPECIFIC)

#elif ! defined(IRIS_MTK_SPECIFIC)

#endif  // IRIS_MTK_SPECIFIC
```



### 用途举例

#### 控制编译

头文件控制

```cpp
#if defined (PXLW_IRIS)

#if defined (IRIS_VERSION_7)
#include "iris7.h"
constexpr int iris_num = 7;
#elif defined (IRIS_VERSION_8)
#include "iris8.h"
constexpr int iris_num = 7;
#elif defined (IRIS_VERSION_NONE)
#include "iris_not_exists.h"
constexpr int iris_num = ABCDEFG;
#else 
#include "qcom.h"
#endif


```



#### 控制打印

只在debug版本打印

把宏的定义改成#define ENABLE_LOG 0，那么就不会满足条件了，也就不会打印日志了。

```cpp
#if NDEBUG
#define CHECK_GL(x) do { \
	(x); \
} while(0)
#else
#define CHECK_GL(x) do { \
	(x); \
	check_gl_error(__FILE__, __LINE__, #x); \
} while(0)
#endif
```





## `#转字符串`  `##拼接(展开)`

```cpp

#define STR(x) #x   //转字符串

//二级
#define CONTACT2(x,y) CONTACT(x,y)
#define STR2(x) STR(x) 

```



### `#` 转字符串

`#`符号：把宏参数，转换为字符串

```cpp
#define MESSAGE(x) #x
const char * str = MESSAGE(test);
// const char * str = "test";
```



### `##` 拼接(展开)

#### 基本使用

`##`符号：连接两边的值，从而产生一个新的值

```cpp
#define CONTACT(x, y) x##y   //拼接
#define CONTACT2(x,y) CONTACT(x,y)  // 对宏进行二次展开


#define MARCO_1front(index) 1##index
int mInt_2 = MARCO_1front(23);	// int mInt_2 = 123;

#define MARCO_1end(index) index##1
int mInt_3 = MARCO_1end(23);   // int mInt_3 = 231;
```



#### 展开规则

这一过程中，左右两边既可以是 输入的参数、常量（字面量）但不可以是**参数之外其它的宏**（否则会不展开）

- 在`##`两侧，只做参数的替换，不进行任何宏的展开
  - **非参数的宏：**
    - **不展开**
  - **参数：**
    - **仅仅会对参数进行替换，不展开**
  
- 不在 `##`两侧，所有的宏都会展开到最终形态（和平时一样）。
  - 非参数的宏：
    - 会多次展开，直到成为最终的样子。
    
      （宏展开成为新的宏，新的宏会继续展开）
    
  - 参数：
    - 会对参数进行替换
    
    - 若替换后仍然是宏，
    
      会多次展开，直到成为最终的样子。

```cpp
// ## 两侧只做参数的替换，不进行任何宏的展开

// 示例:不展开
//  既没有把非参数的INT_当作宏进行展开
//  又没有把参数的ONE当作宏进行展开
#define INT_   pyzhang_
#define ONE		1
#define MARCO_1(index) INT_##index
int MARCO_1(ONE);	// 对应int INT_ONE; 而不是 int pyzhang_1;

#define MARCO_1front(index) 1##index
int mInt_2 = MARCO_1front(23);	// int mInt_2 = 123;

#define MARCO_1end(index) index##1
int mInt_3 = MARCO_1end(23);   // int mInt_3 = 231;

```



####  `##` 两侧二次展开

```cpp
#define CONTACT(x, y) x##y	// 简单拼接
#define CONTACT2(x,y) CONTACT(x,y)  // 宏的二次展开
```

如果需要对 `##` 两侧的宏进行展开，需要使用二级的宏。

```cpp
#define THREE 999
#define TWO THREE
#define ONE TWO

/* 不使用二级的宏 */
#define MARCO_1(param) ONE##param
//  ## 两边的宏(/参数)，是不展开的
MARCO_1(ONE);  // ONE##ONE -> ONEONE 
```

```cpp
/* 使用二级的宏 */
#define CONTACT(x, y) x##y	// 简单拼接
#define CONTACT2(x,y) CONTACT(x,y)  // 宏的二次展开

#define THREE 999
#define TWO THREE
#define ONE TWO


#define MARCO_2(param) CONTACT2(ONE, param)

// 参数展开后传到二级宏，##两侧已经是展开过的宏了
MARCO_2(ONE);  // CONTACT2(ONE, ONE) -> CONTACT(999, 999) -> 999##999 -> 999999
```

#### 示例

下面是一些示例：

```cpp
#define IRIS_ATTR(_name, _mode, _show, _store) \
struct kobj_attribute iris_attr_##_name = __ATTR(_name, _mode, _show, _store)

// 则
static IRIS_ATTR(chip_version, 0644, chip_version_show, chip_version_store);
// 被展开为
struct kobj_attribute iris_attr_chip_version = __ATTR(_name, _mode, _show, _store) // 这里没有展开__ATTR这个宏，虽然不是参数，但是不在##两侧，实际上会继续展开
```

```cpp
#define CONCAT(a, b, c) a##b##c
#define MESSAGE(x) #x

int main() {
    printf("%s\\\\n", MESSAGE(CONCAT(Hello, World, !)));
 // printf("%s\\\\n", "HelloWorld!");
    return 0;
}
```



### `switch case`中使用

#### 打印枚举字符串

```cpp
// 以数字的形式进行打印(展开)
#define PER_GL_ERROR(x) case (x); return (x)

// 以字符串的形式对枚举的名称进行打印(展开)
#define PER_GL_ERROR(x) case (x); return (#x)
// 如果x本身就是一个宏，可以先求值x，变成数值后再打印
#define STR(x) (#x)
#define PER_GL_ERROR(x) case (x); return STR(x)
```

```cpp
std::string get_errno_name(int err) {
    switch(err) {
    PER_GL_ERROR(GL_NO_ERROR);
    PER_GL_ERROR(GL_INVALID_ENUM);
    PER_GL_ERROR(GL_INVALID_VALUE);
    }
    return "unknown error: " + std::tostring(err);
}
```

希望不再重复加前缀

```cpp
// 省略前缀
#define PER_GL_ERROR(x) case GL_##x; return (x)
#define PER_GL_ERROR(x) case GL_##x; return (#x)
```

```cpp
std::string get_errno_name(int err) {

#define PER_GL_ERROR(x) case GL_##x; return (#x)
    switch(err) {
    PER_GL_ERROR(NO_ERROR);		// GL_NO_ERROR
    PER_GL_ERROR(INVALID_ENUM); // GL_INVALID_ENUM
    PER_GL_ERROR(INVALID_VALUE);// GL_INVALID_VALUE
    }
    return "unknown error: " + std::tostring(err);
}
```

延迟宏的求值，避免因为宏的二次指向出现问题。

```cpp
std::string get_errno_name(int err) {
#define CONCAT(x, y) x##y	// 延迟宏的求值，避免因为宏的二次指向出现问题。
#define PER_GL_ERROR(x) case GL_##x; return (#x)
    switch(err) {
    PER_GL_ERROR(NO_ERROR);		// GL_NO_ERROR
    PER_GL_ERROR(INVALID_ENUM); // GL_INVALID_ENUM
	#define newMacro INVALID_VALUE
    PER_GL_ERROR(newMacro);		// GL_INVALID_VALUE
    }
    return "unknown error: " + std::tostring(err);
}
```



#### 更多信息

```cpp
std::string get_errno_name(int err) {
#define CONCAT(x, y) x##y	// 延迟宏的求值，避免因为宏的二次指向出现问题。
#define PER_GL_ERROR(x) case GL_##x; return (#x)
    switch(err) {
    PER_GL_ERROR(NO_ERROR);		// GL_NO_ERROR
    PER_GL_ERROR(INVALID_ENUM); // GL_INVALID_ENUM
	#define newMacro INVALID_VALUE
    PER_GL_ERROR(newMacro);		// GL_INVALID_VALUE
    }
    return "unknown error: " + std::tostring(err);
}
```

```cpp
#define CHECK_GL(x) do \
{\
    get_errno_name(#x);\
    check_gl_error(__FILE__, __LINE__, #x);\
} while (0)

void check_gl_error(const char* filename, int lineno, const char *expr) {
    auto err = glGetError();
    if (err != 0) {
        cout << filename << lineno << expr << "faild" << get_errno_name(err) << endl;
    }
}
```



## `...` 可变参数

> 这个特性可以用来对类似printf这样的函数进行封装。

### 基本使用

`...`传递给 `__VA_ARGS__`

```cpp
// __VA_ARGS__ 接收 可变参数...
// ...传递给 __VA_ARGS__, 调用JNI的内置函数__android_log_print,打印
#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG, TAG, __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, TAG, __VA_ARGS__)
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, TAG, __VA_ARGS__)
```

```cpp
#define LOGD(...) cout << __VA_ARGS__ << endl
```







### `,`问题

#### general

在调用宏时，如果可变参数为空，之前的逗号如果不会删除，导致编译错误，

```cpp
// 错误
#define LOG(format, ...) printf(format, __VA_ARGS__)

LOG("got a number %d", 34); ==>  printf("got a number", 34); // 有参数不报错
trace("got a number");      ==>  printf("got a number",); // 无参数则错误
```

正确做法见如下章节，简单来说

```cpp
#define LOG(format, ...) 	printf(format, ##__VA_ARGS__)
#define PRINT(x, ...) 		printf(format __VA_OPT__(,) __VA_ARGS__)
```



#### `##__VA_ARGS__` 

`##__VA_ARGS__` 是 C/C++ 语言中的宏定义技巧：**用于支持可变长参数**。

- 忽略某个可变参数为空时产生的逗号（将逗号去掉，）从而避免编译错误
- 在`__VA_ARGS__`之前添加`##`符号，使预处理器去除掉它前面的那个逗号。

```cpp
// 正确:解决多参数输入的，无参数输入时的 `,` 问题：
#define LOG(format, ...) printf(format, ##__VA_ARGS__)
```

```cpp
// 这样我们就可以使用我们自己定义的宏 trace 来打印日志了
LOG("got a number %d", 34); ==>  printf("got a number", 34);
LOG("got a number %d"); ==>  printf("got a number");
```





#### ` __VA_OPT__(,)`

C++20的标准库中，支持了这个宏

`__VA_ARGS__`不为空，之中的内容才会显现出来。

```cpp
#define PRINT(x, ...) do { \
	printf(x __VA_OPT__(,) __VA_ARGS__); \
} while(0)
```

```cpp
PRINT("answer is \n"); // 错误，必须有参数
PRINT("answer is %d\n", 98);
PRINT("answer is %d, %d\n", 98, 100);

```









## 预定义宏



### 常用 `__FILE__` etc

除了使用`#define`预处理器来定义宏之外，也可以通过编译器参数来定义宏，

`__FILE__`，`const char*`，当前所编译文件名称（绝对路径）

`__FILE_NAME__`，`const char*`，当前所编译文件名称（仅仅是文件名，如`test.cpp`）

`__LINE__`，`int`，当前所在的行号

`__FUNCTION__`，`const char*`，当前所在函数的名称

`__DATE__`，`const char*`，当前的日期

`__TIME__`，`const char*`，当前的时间

平台

```cpp
const char* system_name() {
    return;
#if defined(__linux__)
        "linux"
#elif defined(__WIN32)
        "windows"
#elif defined(__APPLE__)
        "windows"
#elif defined(__unix__)
        "otherunix"
#else
        "unknown"
#endif
        "unknown"
;
}
```

```cpp
const char* system_name() {
    return;
#if defined(__x86_64__)
        "x86_64"
#elif defined(__i386)
        "i386"
#elif defined(__aarch64__)
        "aarch64"
#elif defined(__arm__)
        "arm"
#else
        "unknown"
#endif
;
}
```



### 判断跨平台

面对不跨平台的宏

```cpp
// 禁止内联优化  gnuc 
__attribute__ ((noinline)) void func(){}
// 禁止内联优化  微软
__declspec(noinline) void func(){}
```

```cpp
#if defined(__MY_MAC)
__attribute__ ((noinline))
#elif defined(__MAC2) || defined(__clang__)
__declspec(noinline)
#endif
void func(){}
```

更棒的写法：

> `!!变量`，将包括指针的任何类型转换为 bool类型。

```cpp
#if defined(__MY_MAC)
#define MY_NOINLINE __attribute__ ((noinline))
#define LIKELY(x)
#define UNLIKELY(x)
#elif defined(__MAC2) || defined(__clang__)
#define MY_NOINLINE __declspec(noinline)
#define LIKELY(x)   __builtin_expect(!!(x)), 1)
#define UNLIKELY(x) __builtin_expect(!!(x)), 0)
#else
#define MY_NOINLINE
#define LIKELY(x)
#define UNLIKELY(x)
#endif

MY_NOINLINE void func(){}
```



### `__PRETTYFUNC__`

可以用于反射，获取函数的参数信息。

#### 展示

```cpp
#define LOG(x) (cout << __FILE__ << ":" << __LINE__ << ": " << __FUNCTION__ << ": " << __PRETTY_FUNCTION__ << ": " << (x) << endl)

#define LOG_TYPE(x) (cout << __PRETTY_FUNCTION__ << ": " << (x) << endl)


```

##### 无模板

```cpp
void func()
{
    LOG_TYPE("func()");
}

void func(int a, int b)
{
    LOG("func(int a, int b)");
    LOG_TYPE("func(int a, int b)");
}

```



```cpp
// 无模板
func();
/*
Cpp_Reflection.cpp:16: func: void func(): func()
void func(): func()
*/


func(1, 2);
/*
Cpp_Reflection.cpp:22: func: void func(int, int): func(int a, int b)
void func(int, int): func(int a, int b)
*/
```



##### 有模板

```cpp
template <class T>
void func(T a)
{
    LOG("func(T a)");
    LOG_TYPE("func(T a)");
}
```

```cpp
// 模板
func(3);
/*
Cpp_Reflection.cpp:29: func: void func(T) [with T = int]: func(T a)
void func(T) [with T = int]: func(T a)
*/

func(3.14f);
/*
Cpp_Reflection.cpp:29: func: void func(T) [with T = float]: func(T a)
void func(T) [with T = float]: func(T a)
*/
```



#### 编译期反射

##### 类型的名称

获取传入模板参数的类型  **实际的类型名**(获取T是什么)。

> 例如：string到底是什么。

```cpp
template <typename T>
string get_type_name()
{
    string s = __PRETTY_FUNCTION__;
    // cout << " s = __PRETTY_FUNCTION__  " << s << endl;
    auto pos = s.find("T = ");
    pos += 4;
    auto pos2 = s.find_first_of(";]", pos);
    return s.substr(pos, pos2 - pos);
}

```

```cpp
cout << get_type_name<int>() << endl;
/*
int
*/
cout << get_type_name<char>() << endl << endl;
/*
char
*/
```



##### get_enum_name (值的名称)

获取传入模板参数的值

- 可以获取传入的enum值的名称
- 以字符串的形式返回

```cpp
template <typename T, T N>
string get_type_value()
// string get_enum_name()
{
    string s = __PRETTY_FUNCTION__;
    // cout << " s = __PRETTY_FUNCTION__  " << s << endl;
    auto pos = s.find("N = ");
    pos += 4;
    auto pos2 = s.find_first_of(";]", pos);
    return s.substr(pos, pos2 - pos);
}
```

```cpp
cout << get_type_value<int, 1>() << endl;
/*
1
*/
cout << get_type_value<char, 'a'>() << endl;
/*
'a'
*/
cout << get_type_value<Color, (Color)2>() << endl;
/*
GREEN
*/
cout << get_type_value<Color, RED>() << endl << endl;
/*
RED
*/
```



##### 编译期不能做的事

实例化 模板

- 尖括号内不可以放运行时的东西。
- 模板会根据参数类型，生成多个实例。

实例化 lambda

- 参数编译和模板函数是一样的，可以根据参数的不同，实例化多次。
- `func(1)`，`func(2)`，`func(3)`等很多遍



#### 运行期反射

反射：运行时确定变量的类型。

对枚举变量而言：给一个枚举变量的值，返回一个其对应的字符串

对某个enum，实例化所有组合。

##### 1 显式多次实例化模板

使用暴力的方法，多次的实例化模板。

目标：

- 希望每个参数`N`的值都会生成对应的实例，实例化多次。

- 从而允许运行期就可以调用对应的实例化了。

如何模板实例化多次？实例化规则之：

- 让变量作为形参，放入尖括号`<>`内

  `<T, T(1)>`，`<T, T(2)>`

- 注意：`(T) 2`是静态的，编译期的变量。

  - 可以放入模板的尖括号内 `myFunc<T, (T) 2>(lambda){}`
  - 会引起对应的模板实例化。

缺点：需要逐个填写判断，特别麻烦。

```cpp
// 反射：运行时确定变量的类型，上面都是编译期确定的
// 获取传入模板参数的值  实际传入的值(值以字符串的形式返回) 可以获取enum的值
template <typename T>
string get_enum_name_dynamic(T n)
{
    // 模板可以生成3个实例
    if (n == (T)1)
        return get_enum_name<T, (T)1>();
    if (n == (T)2)
        return get_enum_name<T, (T)2>();
    if (n == (T)3)
        return get_enum_name<T, (T)3>();
    return "";
}
```

```cpp
Color c1 = RED;
cout << get_enum_name_dynamic(c1) << endl;
cout << get_enum_name_dynamic(RED) << endl << endl;
/*
RED
*/
```



##### 2  static_for

给一个枚举变量的值，返回一个其对应的字符串。

不需要逐个填写判断

- 普通的`for`循环是动态的

  `for(int i = 0; i < ...; i++)`循环

  - `(T)i`是不可以放入模板的尖括号`<>`中的。
  - 其中的`i`是运行时变量，不能放入模板的尖括号内

- 编译期的for循环：`static_for`
  - 可以用来实例化多次模板
  - `static_for<Beg, End>`中，
    - 如`<1,3>`将Beg End作为参数传入，
    - 显式地指定实例化的enum范围/次数。

###### 实现1(if constexpr)

```cpp
template <int N>
struct int_constant // 可以使用 std::intergrad_constant
{
    static constexpr int value = N;
};

template <int Beg, int END, class F>
void static_for(F const &func)
{
    if constexpr (Beg == END)
    {
        return;
    }
    else
    {
        func(int_constant<Beg>());
        static_for<Beg + 1, END>(func);
    }
}
```

###### 实现2(SFAINE)

```cpp
template <
	int Beg, int End,
	class F,
	typename enable_if<Beg != End>::type = 0
>
void static_for_n(F const &func)
{
    // int_constant如果自己写，不使用std提供的intergrad_constant
    // 放在static_for中，可以避免污染当前名称空间。
    struct int_constant
    {
        enum
        {
            value = Beg;
        };
    };
    func(int_constant());
    static_for_n<Beg + 1, End>(func);
}


```

###### 实现3(C++ 98)

static_for_n C++98的实现

```cpp
template <bool Cond>
struct my_enable_if
{
};

template <>
struct my_enable_if<true>
{
    typedef int type;
};

template <class T, T N>
const char *get_enum_name_static()
{
    return __PRETTY_FUNCTION__;
}
template <int Beg, int End, class F, typename my_enable_if<Beg == End>::type = 0>
void static_for_n(
    F const &func) {}

template <int Beg, int End, class F, typename my_enable_if<Beg != End>::type = 0>
void static_for_n(
    F const &func)
{
    struct int_constant
    {
        enum
        {
            value = Beg
        };
    };
    func(int_constant());
    static_for_n<Beg + 1, End>(func);
}


```

```cpp
My::Color c2 = My::RED;
cout << get_enum_name_new(c2) << endl << endl;
/*
RED
*/
```



###### 使用

注意这里的auto，如果使用int则会让lambda成为一个实实在在的函数，只实例化一次。

从而解析`PRETTY_FUNCTION`只能得到一种返回值。

```cpp
template <typename T>
string get_enum_name_dynamic_new(T n)
{
    string ret;
    // C++17以内，lambda只有()参数，C++20支持模板参数
    // [&]<typename T>(T i){} 。
    // int的话，lambda作为一个切切实实的函数只实例化一次.auto才会实例化多次。
    static_for<0, 256>([&](auto i) {
        if (n == (T)i.value)
            ret = get_enum_name<T, (T)i.value>();
    });
    return ret;
}
```

```cpp
cout << get_enum_name_dynamic_new(RED) << endl;
/*
RED
*/
```





##### 反向 由string获知enum int

```cpp
template <typename T, int Beg = 0, int End = 256>
T enum_from_name(string const &s)
{
    for (int i = Beg; i < End; ++i)
    {
        if (s == get_enum_name_new((T)i))
            return (T)i;
    }
    // throw;
    // std::out_of_range();what不直观
    std::cout << "out_of_range" << std::endl;
    throw std::runtime_error("out_of_range");

    // throw;
}
```

```cpp
cout << "case 5   " << enum_from_name<Color>("RED") << endl;
```





## 头文件安全

`ifndef endif`   `#pragma once`

同一个文件只能将同一个头文件包含一次，但是有可能因为包含了另外一个头文件的头文件，而违反这一规则。

为了避免同一头文件被包含多次，`C/C++`中有两种宏实现方式：

解决方案：

- `ifndef endif`
- `#pragma once`

### `#ifndef`

```cpp
/* 
  当且仅当以前没有使用预处理器编译指令#define定义名称COORDIN_H_时
  才处理 #ifndef和#endif之间的语句
  下面的处理#define COORDIN_H_可以保证文件内容只被包含一次
*/
#ifndef  __SOMEFILE_H__ // 或 #if defined(__SOMEFILE_H_)
#define   __SOMEFILE_H__
 ... ... // 声明、定义语句
#endif
```

- 优点
  - 可以保证同一个文件不会被包含多次，
  - 也可以保证内容完全相同的文件不会被不小心同时包含
  - 语言支持所以移植性好
- 缺点：
  - 宏名重复 “撞车”
    - 如果不同头文件中的宏，“撞车”，可能导致头文件明明存在却找不到声明的状况。
  - 编译大型项目时，ifndef会使编译时间相对较长
    - 编译器每次都需要打开头文件才能判定是否有重复定义
    - 并不能防止编译器将文件包含两次，只是让它忽略除第一次包含之外的所有内容。



### `#pragma once`

```cpp
#pragma once
 ... ... // 声明、定义语句
```

- 优点：

  - 可以避免名字冲突

    不需要再担心宏名冲突，不会出现宏名冲突引发的问题

  - 当预处理器第二次读到同一个文件时，就会自动跳过通常头文件都不想被重复导入，

- 缺点：

  - 同一文件不会被包含多次（物理上的同一文件），如果某个头文件存在多个拷贝，不能保证它们不被重复包含
  - 不受一些较老版本编译器的支持







## 使用习惯

### do{ }while(0)

经常见到宏定义中有一个`do{ }while(0)`语句，这是为什么呢？

使用`do{….}while(0)` 把宏包裹起来，成为一个独立的语法单元，从而不会与上下文发生混淆。

同时因为绝大多数的编译器都能够识别do{…}while(0)这种无用的循环并进行优化，所以使用这种方法也不会导致程序的性能降低。

```cpp
// 如果宏的内容很长，可以进行换行，每行的末尾添加`\`，以表明后面的一行依然是宏的内容
// 宏的内容比较长，也没有缩进，易读性较差，因此转为多行
#define ADD(x, y) do { int sum = (x) + (y); return sum; } while (0)
// do{….}while(0) 多行
#define ADD(x, y) \
do \
{\
    int sum = (x) + (y);\
    return sum;\
} while (0)
```

- 避免变量名的重复

比如我们定义一个交换两个值的宏

```text
#define swapint(x, y) int tmp = x; x = y; y = tmp;
```

在大部分情况下可以工作，但是如果之前已经定义了tmp这个变量，则就会出错了，那我们可以把tmp换成平时不常用的名字，就大大降低了重名的概率了，这确实是一个办法，但不完美。因为即使这样，依然无法用在switch语句中。case语句中不允许声明变量

```cpp
int x = 1, y = 2;
switch (value)
{
    case 1:
        // 编译出错，因为case语句中不允许声明变量
        swapint(x, y);
        break;
}
```

- 支持`switch`语句

定义宏的时候，加上一层大括号，这样便可以用在`switch`语句中了。

但是会与`;`使用习惯相悖，多加一个`;`会导致编译报错。也影响IDE的自动代码格式化。

```cpp
#define swapint(x, y) {int tmp = x; x = y; y = tmp;}
```

```text
int x = 1, y = 2;
if (x < y)
    swapint(x, y);
else
    someaction();
// 上面的代码展开
if (x < y)
    {int tmp = x; x = y; y = tmp;};
else
    someaction();
// 编译出错，因为在else之前多了一个分号，导致语法错误，那么能不能不加分号
// 可以，但是C++程序员一般都习惯在末尾添加分号，而且不过不加分号，也会影响
// IDE的自动代码格式化
```



### 定义时 `(x)`

在宏定义时，给宏加上括号，参数也都加上括号

##### (宏)

给宏加上括号：避免 **运算的优先级被改变**。

```cpp
// 正确的宏定义
#define ADD(x, y) ((x) + (y))
// 错误的宏定义
#define ADD(x, y) (x) + (y)
```

```cpp
// 输入
int value = ADD(2, 3) * ADD(4, 5);
```

```cpp
// 正确的宏定义 输出:  正确
int value = ((2) + (3)) * ((4) + (5));	// 45

// 错误的宏定义 输出:  运算级错误
int value = (2) + (3) * (4) + (5);		// 19
```

##### (参数)

给参数加括号：避免 **宏参数中含有逗号**  ，从而错误分割了参数。

> 宏可以携带参数，而参数并没有什么要求，如果宏参数中含有逗号，那么就会带来歧义了

```cpp
// 该宏本身没什么实际使用意义，只是为了说明问题
#define segment(seg) seg

// 宏参数中不含有逗号，没有问题
segment(int x = 1; int y = 3);

// 宏参数中含有逗号，编译错误，
// 因为宏展开时把","视为参数间的分隔符，但是这里的宏只接受一个参数
segment(int x = 1, y = 3);

// 解决办法就是给宏参数加上括号，使其为一体
segment((int x = 1, y = 3));
```



### 重复求值 auto

对于需要重复求值的情况

do while 没有返回值 ++ 

linux

```cpp
#define ASSERT_GT(x,y) do{\
	auto ___x = (x);\
	if (!(__x) > (y)) { \
		std::cerr << "Assert failed : " << #x << "(" << __x < < ")" <<  
	}\
```

windows

```cpp
typeof(x) __x = (x);
```





## 宏的调试

### 使用IDE

IDE会自动检查宏的展开情况。



### 编译时打印信息

`#pragam message` debug

如果想要在编译时打印一些信息，可以使用如下预处理指令：

```cpp
// 在编译时报告和显示信息。
// 在编译期间，将一个文字串（messagestring）发送到标准输出窗口。
#pragma message("will print this message")
```

如果宏太过于复杂需要调试，可以利用宏自身的特性把宏展开后的内容打印出来。

- 如果想要打印某个宏的内容，会发现编译器会报错。

- 难点：宏不支持在运行时调试。
- 原因：`#pragma message`指令必须接收一个字符串，

解决方法：

可使用如下代码协助输出SOMEMACRO的内容。

```text
#define SOMEMACRO 123456

#define MACROTOSTR2(x) #x
#define PRINTMACRO(x) #x " = " MACROTOSTR2(x)

#pragma message(PRINTMACRO(SOMEMACRO))
```

编译上述代码便会在输出窗口打印`SOMEMACRO = 123456`的内容。

```sh
#pragma message: SOMEMACRO = 123456
```

带参数的宏同样适用：

```text
#define SOMEMACRO 123456

#define MACROPARAM(x) new int(x);
#define MACROTOSTR2(x) #x
#define PRINTMACRO(x) #x " = " MACROTOSTR2(x)

#pragma message(PRINTMACRO(MACROPARAM(SOMEMACRO)))
```

```sh
# 上述代码块在编译时会打印出
MACROPARAM(SOMEMACRO) = new int(123456); 
#pragma message: MACROPARAM(SOMEMACRO) = new int(123456);

#就是宏展开后的内容。
```

当宏出现问题时，可以使用该方法打印出宏展开后的内容，然后调试展开后的内容，找到错误原因，接着同步修正宏本身的错误。



