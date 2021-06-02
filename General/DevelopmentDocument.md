# TicketSystem:LaMétropole

## 模块划分及功能简介

#### 数据库部分

* 封装的文件读写类
* 封装的模板BPT

--------

#### 功能部件

* L_time
* 各作为多关键字key的结构体
* parser
* vector、unordered_map等容器
* hash、sort等函数

--------

#### 后端逻辑

* user相关操作
* train与ticket相关操作
* 整合两部分的ticketSystem类

----------------------

## 类设计

#### namespace LaMetropole

以下所有的类都包含在LaMetropole的名字空间内，以避免与std内的重名。

### paser相关类

* **paser**

  用于接受命令行，返回处理后的信息。

  包含在paser类内的子类:

  * **paser::PaperCup**

    用于保存处理后的信息

    包含成员变量：

    * `char arv`参数总数 
    * `string* arg[26]`  `-(i+'a')`对应的指令
    * `char key[26]`各参数对应的指示符

  * **paser::tokenScanner**

    用于将一个字符串按给定的分割符分割。

    包含成员函数：

    * `bool hasNextToken()`
    * `string* nextToken()`

  成员函数：

  * `PaperCup* listen()`
    * 读取一行命令，处理后以`paser::PaperCup`的格式返回

### user相关类

* **user**

  用于储存用户的信息

  * 包含成员变量
    * `char username[]`
    * `char password[]`
    * `char priviege`
    * `char name[]`
    * `char mailAddr[]`
  * 构造函数
    
  
* **userManager**

  * 处理与用户有关的操作
  * 包含成员变量
  
* 用于储存用户信息及提供检索的BPT
    * 用于维护用户登录态的unordered_map
    
  * 包含成员函数

    * `void addUser(LaMetropole::paser::PaperCup*)`

      增加一名用户

    * `void login(LaMetropole::paser::PaperCup*)`

      登录一名用户

    * `void logout(LaMetropole::paser::PaperCup*)`

      登出一名用户

    * `void queryProfile(LaMetropole::paser::PaperCup*)`

      查询一名用户的信息

    * `void modifyProfile(LaMetropole::paser::PaperCup*)`
  
      修改一名用户的信息

### Train相关类

* **trainManager**

  * 处理与火车相关的操作

  * 包含成员变量

    * 用于储存

  * 包含成员函数

    * `void addTrain(LaMetropole::paser::PaperCup*)`

      增加一辆火车

    * `void releaseTrain(LaMetropole::paser::PaperCup*)`

      释放一辆火车

    * `void queryTrain(LaMetropole::paser::PaperCup*)`

      询问符合条件的火车

    * `void deleteTrain(LaMetropole::paser::PaperCup*)`

      删除一辆火车

    * `void queryTransfer(LaMetropole::paser::PaperCup*)`

      询问符合条件的换乘路线

    * `void queryTicket(LaMetropole::paser::PaperCup*)`

      询问票

    * `void buyTicket(LaMetropole::paser::PaperCup*)`

      买票

    * `void queryOrder(LaMetropole::paser::PaperCup*)`

      询问订单

    * `void refundTicket(LaMetropole::paser::PaperCup*)`

    * `void Clean(LaMetropole::paser::PaperCup*)`

    * `void Exit(LaMetropole::paser::PaperCup*)`

### 数据库相关类

* **MemoryRiver**

  用于文件读写的类，支持空快的回收

  增加了可以在编译时选择开启的cache功能

  * 实例化参数`template<class T, int info_len, int poolSize>`
    * T为存储的类型
    * info_len为头部信息量，default=2，用作维护空块链表，实现空块回收
    * poolSize是cache的大小

  **cache策略：**

  * 基于外存块的读写频率进行缓存
  * 用一个小根堆来维护cache中的各块的访问频率
  * cache满时比较新块与堆根部的访问频率，如果新块的频率更大，则pop后push新块

* **BPT**

  不支持重复key，使用`MemoryRiver`类维护文件读写的BPlusTree

  * 实例化参数`template<class T,class U,int M,int L,class Compare=std::less<T>`：
    * T为作为Key值的变量
    * U为储存的变量
    * M为非叶节点的大小
    * L为叶节点的大小
    * Compare为比较函数
  * 使用哨兵元素支持重复key
    * 以双关键字变量为key，对每个不同的第一关键字插入一个取第二关键字不会取到的最小值的哨兵哨兵元素，当寻找第一关键字的所有元素时，先找到哨兵元素，再往一个方向通过叶子节点的链表来遍历寻找到所有元素。

### 映射相关类

* **unordered_map**

  模仿std::unordered_map，含有operator[]等函数（不含迭代器）

* **map**

  模仿std::map，用红黑树实现

### 其他功能类

* pair

  模仿std::pair

## 文件设计

#### 源文件

* src

  * main.cpp 主程序

  * includes

    * Algorithm.h & Algorithm.cpp

      一些小函数，如sort

    * BPT.hpp

      BplusTree （Libro）

    * BPlusTree （Mzi）

    * exceptions.hpp

      异常类

    * L_time.h & L_time.cpp

      时间类，及其他作为key的结构体

    * map.hpp

      仿std

    * parser.h & parser.cpp

      处理输入

    * ticketSystem.h & ticketSystem.cpp

      整合各部分的综合类

    * train.h & train.cpp

      火车类及trainManager

    * user.h & user.cpp

      用户类及userManager

    * utility.h & utility.cpp

      仿std

    * vector.hpp

      仿std

* cmakeLists.txt

#### 存储文件

* “train.file”

  用来存储车辆信息

* "ID_crystal.file", "ID_index.file"

  用来存用户信息的BPT的两个子文件，crystal后缀为非叶节点，index后缀是叶节点（下略）

* "stationID_crystal.file", "stationID_index.file"

  BPT的两个子文件，实现`station_name + train_offset -> train_offset + number in the train` 的映射 

* "Arya_crystal.file", "Arya_index.file"

  BPT的两个子文件，实现`trainID + departing_day + pending_number -> pending record` 的映射 

* "Yuki_crystal.file", "Yuki_index.file"

  BPT的两个子文件，实现`trainID + departing_day -> offset of seatArray` 的映射 

* "seat.file"

  用来存储seatArray

* "user_crystal.file", "user_index.file"

  BPT的两个子文件，实现`userID -> user information` 的映射 

* "Sabine_crystal.file", "Sabine_index.file"

  BPT的两个子文件，实现`userID + timeTag -> user's order record` 的映射 

## 组员工作情况

Libro：完成后端逻辑 2021/6/2