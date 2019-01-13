RESTORE FILELISTONLY 
FROM DISK = 'E:\hpkj数据库\qpaccountsdb.bak' --备份文件存放路径

--正在将数据库 'qpaccountsdb' 从版本 661 转换为当前版本 782。
RESTORE DATABASE qpaccountsdb
FROM DISK = 'E:\hpkj数据库\qpaccountsdb.bak'   --bak文件路径
with replace,
MOVE 'data1' TO 'E:\hpkj数据库\qpaccountsdb.mdf',   --mdf文件路径
MOVE 'log' TO 'E:\hpkj数据库\qpaccountsdb_1.ldf'   --ldf文件路径


RESTORE FILELISTONLY 
FROM DISK = 'E:\hpkj数据库\qpgameweb.bak' --备份文件存放路径

--正在将数据库 'qpgameweb' 从版本 661 转换为当前版本 782。
RESTORE DATABASE qpgameweb
FROM DISK = 'E:\hpkj数据库\qpgameweb.bak'   --bak文件路径
with replace,
MOVE 'data1' TO 'E:\hpkj数据库\qpgameweb.mdf',   --mdf文件路径
MOVE 'log' TO 'E:\hpkj数据库\qpgameweb_1.ldf'   --ldf文件路径

RESTORE FILELISTONLY 
FROM DISK = 'E:\hpkj数据库\qpplatformdb.bak' --备份文件存放路径

--正在将数据库 'qpplatformdb' 从版本 661 转换为当前版本 782。
RESTORE DATABASE qpplatformdb
FROM DISK = 'E:\hpkj数据库\qpplatformdb.bak'   --bak文件路径
with replace,
MOVE 'data1' TO 'E:\hpkj数据库\qpplatformdb.mdf',   --mdf文件路径
MOVE 'log' TO 'E:\hpkj数据库\qpplatformdb_1.ldf'   --ldf文件路径


RESTORE FILELISTONLY 
FROM DISK = 'E:\hpkj数据库\qprecorddb.bak' --备份文件存放路径

--正在将数据库 'qprecorddb' 从版本 661 转换为当前版本 782。
RESTORE DATABASE qprecorddb
FROM DISK = 'E:\hpkj数据库\qprecorddb.bak'   --bak文件路径
with replace,
MOVE 'data1' TO 'E:\hpkj数据库\qprecorddb.mdf',   --mdf文件路径
MOVE 'log' TO 'E:\hpkj数据库\qprecorddb_1.ldf'   --ldf文件路径

RESTORE FILELISTONLY 
FROM DISK = 'E:\hpkj数据库\qptreasuredb.bak' --备份文件存放路径

--正在将数据库 'qptreasuredb' 从版本 661 转换为当前版本 782。
RESTORE DATABASE qptreasuredb
FROM DISK = 'E:\hpkj数据库\qptreasuredb.bak'   --bak文件路径
with replace,
MOVE 'data1' TO 'E:\hpkj数据库\qptreasuredb.mdf',   --mdf文件路径
MOVE 'log' TO 'E:\hpkj数据库\qptreasuredb_1.ldf'   --ldf文件路径

RESTORE FILELISTONLY 
FROM DISK = 'E:\hpkj数据库\luckmedb.bak' --备份文件存放路径

--正在将数据库 'QPGameUserDB' 从版本 661 转换为当前版本 782。
RESTORE DATABASE luckmedb
FROM DISK = 'E:\hpkj数据库\luckmedb.bak'   --bak文件路径
with replace,
MOVE 'data1' TO 'E:\hpkj数据库\luckmedb.mdf',   --mdf文件路径
MOVE 'log' TO 'E:\hpkj数据库\luckmedb_1.ldf'   --ldf文件路径