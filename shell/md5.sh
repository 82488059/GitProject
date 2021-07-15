#! /bin/bash

# ln -s /var/mdca/ltr_actuator/mdcamd5.sh /usr/bin/mdcamd5.sh

function read_dir(){
for file in `ls $1` #注意此处这是两个反引号，表示运行系统命令
do
 if [ -d $1"/"$file ] #注意此处之间一定要加上空格，否则会报错
 then
 read_dir $1"/"$file
 else
 #echo $1"/"$file #在此处处理文件即可
 echo `md5sum $1"/"$file` 
 fi
done
}

function run(){
if [ -d $1 ]
then
read_dir $1
else
echo 'md5sum'
echo `md5sum $1`
fi
}

#读取第一个参数
#read_dir $1
run $1

