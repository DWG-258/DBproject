cd ../bin
rm -rf ../data/test_db/test.tbl
echo -e "use test_db;\ncreate table test (id int primary key,name string,score double);" | ./dblite > ../test/drop_table.txt
#加上-Z選項可以忽略空格和制表符的差異
if diff -Z ../data/test_db/test.tbl ../test/status1.ref > /dev/null; then
    echo "create table test passed"
else
    echo "create table test failed"
fi

cd ../test
