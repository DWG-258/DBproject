cd ../bin
echo -e "use test_db;\ndrop table test;" | ./dblite > ../test/drop_table.txt
if [ ! -f "../data/test_db/test.tbl" ]; then
    echo "drop table test passed"
else
    echo "drop table test failed"
fi
cd ../test