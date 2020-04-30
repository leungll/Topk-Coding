# Topk-Coding
![maven](https://img.shields.io/badge/C%2B%2B-passing-red)
![maven](https://img.shields.io/badge/Python-2-blue)
![maven](https://img.shields.io/badge/python-3-yellow)
# Environment
* Linux
* python2 **( e.g. python2.7 )**：跑 berlowitz 代码
* python3：跑 `kplexes.py`
* c++11
* 枚举kplex部分需要安装 **[snap.py](https://snap.stanford.edu/snappy/)** 库，`snap.py` 库对环境要求很高
* 实验代码已在 `ubtuntu 18.04` 系统执行通过

# Prepare
* 关于python2
    - 如果你的 python2 不是 2.7 版本，需要在 `kplexes.py`(大概在 108 行，路径：`Topk-Coding/kplexes.py` )的 `call` 函数中修改对应的 `py2` 版本号
    [ `call("python2.7 %s %s" % (berexe, argstring), shell=True, stdout=DEVNULL)` ]
* 安装 **[snap.py](https://snap.stanford.edu/snappy/)** 库
    - 可直接命令行安装：`pip install snap-stanford`
    - 下载：**https://snap.stanford.edu/snappy/release/**
    - 执行 **( e.g. `ubtuntu 18.04 & py2.7` )**：
        ```
        tar zxvf snap-stanford-5.0.0-5.0-ubuntu18.04.2-x64-py2.7.tar.gz
        cd snap-stanford-5.0.0-5.0-ubuntu18.04.2-x64-py2.7
        sudo python2.X setup.py install

        ```
        ![JqrUe0.png](https://s1.ax1x.com/2020/04/30/JqrUe0.png)
# Compile & Run

```
cd Topk-Coding
mkdir working_dir
mkdir input_data
mkdir input_data_c
mkdir ans
g++ -std=c++11 Coding.cpp -o Coding
./Coding XXX.input_file k
```

# Common Commands
* 查看系统发行版本：`cat /etc/redhat-release`
* 查看 python 具体版本：`python --version`
* 查看 python 具体安装路径：`which python`
* 查看 pip 具体版本： `pip --version`
* 查看 pip 具体安装路径：`which pip`
* tar.gz 解压：`tar zxvf XXX.tar.gz`
* zip 解压：`unzip XXX.zip`
* vim 编辑：
    ```
    vim /usr/bin/yum 
    i 
    ESC 
    :wq / :q!
    ```
* 查看系统 python 版本 ：`ll /usr/bin/python*`
* 备份 python 原版本：`mv /usr/bin/python /usr/bin/python.bak`
* 建立 python 软链接：`ln -s /usr/local/python27/bin/python2.7 /usr/bin/python`
* 恢复原来的 python 指向：`cp /usr/bin/python.bak /usr/bin/python`
* pip 国内镜像源：
    - 清华：`pip install 包名 -i https://pypi.tuna.tsinghua.edu.cn/simple `
    - 阿里云：`pip install 包名 -i http://mirrors.aliyun.com/pypi/simple/`
    - 豆瓣源（ python 指定 pip ）：`python -m pip install 包名  -i http://pypi.douban.com/simple/ --trusted-host pypi.douban.com`

# Pay Attention
* `Coding.cpp` 中的 `ss_python_cmd` 文件后缀名要与 `kplexes.py` 中的 `extension` 匹配
* 执行命令前需要创建四个文件夹，否则会报错
    - mkdir working_dir
    - mkdir input_data
    - mkdir input_data_c
    - mkdir ans

# Update kplexes.py
* 修改命令行参数（ `argparse` & `parser` ）
* 注释
    - 169行：`print("%s remove %d edges" % (datetime.today().strftime('%Y-%m-%d %H:%M:%S'), len(cond)))`
    - 246行：`print("%s started %d threads (batch_size=%d, procnum=%d)" % (datetime.today().strftime('%Y-%m-%d %H:%M:%S'), i+1, batch_size, procnum), file = sys.stderr)`
    - 250行：`print("%s ended thread %d" % (datetime.today().strftime('%Y-%m-%d %H:%M:%S'), i), file = sys.stderr)`
* 增加文件比较
    - 29行：`fileNameStr = args.fileName`
    - 297行：`if filename == fileNameStr`
* K 从命令行接收：
    - 27行：`k_cmd = args.k`
    - 304行：`for k in [k_cmd]`
* 注释了 m 的运算规则，从命令行接收：
    - 318行：`m  = max(k**2, math.ceil(fatt))`
    - 401行：`m  = max(k**2, math.ceil(fatt))`
    - 28行：`m = args.m`
* 注释了 413 行：`print(P, file=sys.stderr)`