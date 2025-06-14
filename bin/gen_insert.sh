#!/bin/bash

# 输出 SQL 到 insert_test.sql
echo "use test1;" > insert_test.sql

for i in $(seq 5 100); do
    name="student_$i"
    score=$(echo "scale=1; 60 + $i * 0.5" | bc)
    echo "insert into testindex values ($i,\"$name\",$score);" >> insert_test.sql
done

# 可选：添加一个查询命令以检查插入是否成功
echo "select * from testindex;" >> insert_test.sql
echo "select * from testindex where id = 99;" >> insert_test.sql
