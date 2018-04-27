import os
import cv2
import json
def find_last(string, str):
    last_position = -1
    while True:
        position = string.find(str, last_position + 1)
        if position == -1:
            return last_position
        last_position = position

def str2float(s):  
    def char2num(s):  
        return {'0': 0, '1': 1, '2': 2, '3': 3, '4': 4, '5': 5, '6': 6, '7': 7, '8': 8, '9': 9}[s]  
    n = s.index('.')  
    return reduce(lambda x,y:x*10+y,map(char2num,s[:n]+s[n+1:]))/(10**n)  

with open('/media/pengshanzhen/bb42233c-19d1-4423-b161-e5256766be8e/1_xuzhneghao/data/04.25red_g_d/0424up/jsonlist.txt') as jslist:
    lines=jslist.readlines()
for line in lines:
    name = line[:-1][find_last(line[:-1], '/') + 1:-9]
    jsfile=open(line[:-1])
    jsf=json.load(jsfile)
    obj=jsf['objects']
    img=cv2.imread('/media/pengshanzhen/bb42233c-19d1-4423-b161-e5256766be8e/1_xuzhneghao/data/04.25red_g_d/0424up/mark/'+name+'.jpg')
    imgsz=[img.shape[1],img.shape[0]]
    Size=[300,300]
    MaxlightSize=150
    num_row=(imgsz[0]-MaxlightSize)/(Size[0]-MaxlightSize)+1
    num_col=(imgsz[1]-MaxlightSize)/(Size[1]-MaxlightSize)+1
    for i in range(0,num_row):
        for j in range(0,num_col):
            #j=0
            imgrect=[min(i*(Size[0]-MaxlightSize),imgsz[0]-Size[0]),min(j*(Size[1]-MaxlightSize),imgsz[1]-Size[1]),Size[0],Size[1]]
            imgcroped=img[min(j*(Size[1]-MaxlightSize),imgsz[1]-Size[1]):min(j*(Size[1]-MaxlightSize),imgsz[1]-Size[1])+Size[1],min(i*(Size[0]-MaxlightSize),imgsz[0]-Size[0]):min(i*(Size[0]-MaxlightSize),imgsz[0]-Size[0])+Size[0],:]            
            num=0
            datalist=[]
            for x in obj:
                lbrect=x['rect']
                lb=x['label']
                if lbrect[2]>lbrect[3]:
                    lb=lb+'-heng'
                else:
                    lb=lb+'-shu'
                #print lbrect[0],lbrect[1],lbrect[2]
                x_new=float(lbrect[0])-imgrect[0]
                y_new=float(lbrect[1])-imgrect[1]
                w_new=float(lbrect[2])#-imgrect[2]
                h_new=float(lbrect[3])#-imgrect[3]
                if x_new>=-5 and y_new>=-5 and x_new+w_new<=Size[0]+5 and y_new+h_new<=Size[1]+5:                    
                    num=num+1
                    newlist=[lb,str(int(x_new)),str(int(y_new)),str(int(x_new+w_new)),str(int(y_new+h_new))]
                    datalist.append(newlist)
                    #savetxt.write(lb+' '+str(x_new)+' '+str(y_new)+' '+str(x_new+w_new)+' '+str(y_new+h_new)+'\n')
                    #cv2.rectangle(imgcroped,(int(x_new),int(y_new)),(int(x_new+w_new),int(y_new+h_new)),(0,0,255),2)
                    #print name,i,j,x_new,y_new,w_new,h_new
            if len(datalist)>0:
                #print num
                savetxt=open('/media/pengshanzhen/bb42233c-19d1-4423-b161-e5256766be8e/SSD/trafficlight/shanzhen/crop/2/try2/label/'+name+'_300_'+str(i)+'_'+str(j)+'.txt','w')
                savetxt.write(str(num)+'\n')
                for t in range(0,len(datalist)):
                    #print datalist[t]
                    savetxt.write(datalist[t-1][0]+' '+datalist[t-1][1]+' '+datalist[t-1][2]+' '+datalist[t-1][3]+' '+datalist[t-1][4]+'\n')
            cv2.imwrite('/media/pengshanzhen/bb42233c-19d1-4423-b161-e5256766be8e/SSD/trafficlight/shanzhen/crop/2/try2/img/'+name+'_300_'+str(i)+'_'+str(j)+'.jpg',imgcroped)
