cd ../bin
rm -rf ../data/test_db
echo -e "create database test_db;" | ./dblite > ../test/create_database.txt
if [ -d "../data/test_db" ]; then
    echo "create database test passed"
else
    echo "create database test failed"
fi
cd ../test