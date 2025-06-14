#!/bin/bash

# 生成 select 查询语句
query_file="../test/select_10k.in"
echo "use test_db;" > "$query_file"
echo "select * from test;" >> "$query_file"

echo "[info] 查询语句生成完毕：$query_file"

# 执行查询并统计耗时
cd ../bin || exit 1
command=$(cat ../test/select_10k.in)

start_time=$(date +%s%N)
echo -e "$command" | ./dblite > ../test/select_10k_result.txt
end_time=$(date +%s%N)

elapsed_ms=$(( (end_time - start_time) / 1000000 ))
echo "[info] 查询执行耗时：$elapsed_ms 毫秒"

# 统计查询到的行数
row_count=$(grep -c "user_" ../test/select_10k_result.txt)

row_count=$((row_count - 1))

if [ "$row_count" -eq 10000 ]; then
    echo "[pass] select 10k test passed, total rows: $row_count"
else
    echo "[fail] select 10k test failed, only $row_count rows returned"
fi
