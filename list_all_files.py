#coding=utf-8
import sys
import os  
f=open('/media/d/work/weizhang/oridata/20180423-违章/biandao.txt','w')
def list_all_files(now_dir):  
    if os.path.isfile(now_dir):  
        print now_dir
        if str(now_dir)[-4:]=='.jpg':
           f.write(str(now_dir)+'\n')  
    else:  
        listdir = os.listdir(now_dir)  
        for i in listdir:  
            if os.path.isfile(i):   
                print i                 
            else:  
                i = now_dir + '/' + i  
                list_all_files(i)  
#f.close()  
list_all_files(sys.argv[1])  