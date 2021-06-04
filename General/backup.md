## 功能

#### 全量备份（首次/自动）

#### 增量备份

* 和最近一次全量备份比较自动进行增量备份
* 回滚

#### 恢复日志

* 命令储存文件
* 命令缓存
* 从文件中读取命令进行回退
  * 只用记录影响数据库的命令，查询命令可以忽略
    * add_user
    * modify_profile
    * add_train
    * delete_train
    * release_train
    * buy_ticket
    * refund_ticket
    * clean
* 可以根据缓存命令来回退