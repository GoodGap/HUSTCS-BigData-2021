# 你写的命令将在linux的命令行运行(test1_2.sh)
# 利用备份文件residents_bak.sql还原数据库:
gs_restore -U gaussdb -W 'Passwd123@123' residents_bak.sql -p 5432 -d residents