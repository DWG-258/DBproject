cd ../bin
echo -e "use test_db;\ndelete test where name = "Tom";" | ./dblite > ../test/delete.txt
file="../data/test_db/test.tbl"
if grep -q "Tom" "$file";then
    echo "delete test failed"
else
    echo "delete test passed"
fi
cd ../test