#coding=utf-8
testFile = open('/media/d/work/weizhang/data/04231', 'w')
trainFile = open('/media/d/work/weizhang/data/0423o', 'w')
num=0
with open('/media/d/work/weizhang/data/0423a') as f:
    lines=f.readlines()
for line in lines:
    num=num+1
    if num%4 ==3:
        testFile.write(line)
    else:
        trainFile.write(line)
testFile.close()
trainFile.close()
