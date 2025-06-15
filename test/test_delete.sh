cd ../bin
echo -e "use test_db;\ndelete from test where name = "Tom";" | ./dblite > ../test/output/delete.txt
file="../data/test_db/test.tbl"
if grep -q "Tom" "$file";then
    echo "delete test failed"
else
    echo "delete test passed"
fi
cd ../test