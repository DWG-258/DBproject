#!/bin/bash


cd ../bin || exit 1
command=$(cat ../test/input/insert_10k.in)

start_time=$(date +%s%N)
echo -e "$command" | ./dblite > ../test/output/insert_10k_result.txt
end_time=$(date +%s%N)

elapsed_ms=$(( (end_time - start_time) / 1000000 ))

echo "[info] 插入执行耗时：$elapsed_ms 毫秒"

file="../data/test_db/test.tbl"

if grep -q "1 user_1" "$file" && \
   grep -q "5000 user_5000" "$file" && \
   grep -q "10000 user_10000" "$file"; then
    echo "[pass] insert 10k test passed"
else
    echo "[fail] insert 10k test failed"
fi
