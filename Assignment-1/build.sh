echo "清除旧版本文件..."
rm ./build/*.pdf ./build/*.cpp ./build/*.zip

echo "拷贝文件..."
cp ./code/baseline.cpp ./build/base.cpp
cp ./code/no_even.cpp ./build/optimizer1.cpp
cp ./code/no_cast.cpp ./build/optimizer2.cpp
cp ./code/better_cache.cpp ./build/optimizer3.cpp
cp ./code/final_version.cpp ./build/optimizer4.cpp

cp ./doc/mpi.pdf ./build/2018011205014_杨睿妮.pdf

echo "打包中..."
cd ./build
zip -r ./build.zip ./*
mv build.zip 2018011205014_杨睿妮.zip