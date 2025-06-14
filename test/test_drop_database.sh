cd ../bin
echo -e "drop database test_db;" | ./dblite > ../test/drop_database.txt
if [ ! -d "../data/test_db" ]; then
    echo "drop database test passed"
else
    echo "drop database test failed"
fi
cd ../test