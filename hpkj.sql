RESTORE FILELISTONLY 
FROM DISK = 'G:\soft\hpkj���ݿ�\qpaccountsdb.bak' --�����ļ����·��

--���ڽ����ݿ� 'qpaccountsdb' �Ӱ汾 661 ת��Ϊ��ǰ�汾 782��
RESTORE DATABASE qpaccountsdb
FROM DISK = 'G:\soft\hpkj���ݿ�\qpaccountsdb.bak'   --bak�ļ�·��
with replace,
MOVE 'data1' TO 'G:\soft\hpkj���ݿ�\qpaccountsdb.mdf',   --mdf�ļ�·��
MOVE 'log' TO 'G:\soft\hpkj���ݿ�\qpaccountsdb_1.ldf'   --ldf�ļ�·��


RESTORE FILELISTONLY 
FROM DISK = 'G:\soft\hpkj���ݿ�\qpgameweb.bak' --�����ļ����·��

--���ڽ����ݿ� 'qpgameweb' �Ӱ汾 661 ת��Ϊ��ǰ�汾 782��
RESTORE DATABASE qpgameweb
FROM DISK = 'G:\soft\hpkj���ݿ�\qpgameweb.bak'   --bak�ļ�·��
with replace,
MOVE 'data1' TO 'G:\soft\hpkj���ݿ�\qpgameweb.mdf',   --mdf�ļ�·��
MOVE 'log' TO 'G:\soft\hpkj���ݿ�\qpgameweb_1.ldf'   --ldf�ļ�·��

RESTORE FILELISTONLY 
FROM DISK = 'G:\soft\hpkj���ݿ�\qpplatformdb.bak' --�����ļ����·��

--���ڽ����ݿ� 'qpplatformdb' �Ӱ汾 661 ת��Ϊ��ǰ�汾 782��
RESTORE DATABASE qpplatformdb
FROM DISK = 'G:\soft\hpkj���ݿ�\qpplatformdb.bak'   --bak�ļ�·��
with replace,
MOVE 'data1' TO 'G:\soft\hpkj���ݿ�\qpplatformdb.mdf',   --mdf�ļ�·��
MOVE 'log' TO 'G:\soft\hpkj���ݿ�\qpplatformdb_1.ldf'   --ldf�ļ�·��


RESTORE FILELISTONLY 
FROM DISK = 'G:\soft\hpkj���ݿ�\qprecorddb.bak' --�����ļ����·��

--���ڽ����ݿ� 'qprecorddb' �Ӱ汾 661 ת��Ϊ��ǰ�汾 782��
RESTORE DATABASE qprecorddb
FROM DISK = 'G:\soft\hpkj���ݿ�\qprecorddb.bak'   --bak�ļ�·��
with replace,
MOVE 'data1' TO 'G:\soft\hpkj���ݿ�\qprecorddb.mdf',   --mdf�ļ�·��
MOVE 'log' TO 'G:\soft\hpkj���ݿ�\qprecorddb_1.ldf'   --ldf�ļ�·��

RESTORE FILELISTONLY 
FROM DISK = 'G:\soft\hpkj���ݿ�\qptreasuredb.bak' --�����ļ����·��

--���ڽ����ݿ� 'qptreasuredb' �Ӱ汾 661 ת��Ϊ��ǰ�汾 782��
RESTORE DATABASE qptreasuredb
FROM DISK = 'G:\soft\hpkj���ݿ�\qptreasuredb.bak'   --bak�ļ�·��
with replace,
MOVE 'data1' TO 'G:\soft\hpkj���ݿ�\qptreasuredb.mdf',   --mdf�ļ�·��
MOVE 'log' TO 'G:\soft\hpkj���ݿ�\qptreasuredb_1.ldf'   --ldf�ļ�·��

RESTORE FILELISTONLY 
FROM DISK = 'G:\soft\hpkj���ݿ�\luckmedb.bak' --�����ļ����·��

--���ڽ����ݿ� 'QPGameUserDB' �Ӱ汾 661 ת��Ϊ��ǰ�汾 782��
RESTORE DATABASE luckmedb
FROM DISK = 'G:\soft\hpkj���ݿ�\luckmedb.bak'   --bak�ļ�·��
with replace,
MOVE 'data1' TO 'G:\soft\hpkj���ݿ�\luckmedb.mdf',   --mdf�ļ�·��
MOVE 'log' TO 'G:\soft\hpkj���ݿ�\luckmedb_1.ldf'   --ldf�ļ�·��