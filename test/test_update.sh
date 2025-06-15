cd ../bin
echo -e "use test_db;\nupdate test set score = 88.4 where name = "user_11";" | ./dblite > ../test/output/update.txt
file="../data/test_db/test.tbl"
if grep -q "11 user_11 88.4" "$file"; then
    echo "update test passed"
else 
    echo "update test failed"
fi
cd ../test