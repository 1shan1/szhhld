import os

def find_last(string, str):
    last_position = -1
    while True:
        position = string.find(str, last_position + 1)
        if position == -1:
            return last_position
        last_position = position

savetxt=open('/media/pengshanzhen/bb42233c-19d1-4423-b161-e5256766be8e/SSD/trafficlight/shanzhen/crop/2/try2/train_0424.txt','w')
with open('/media/pengshanzhen/bb42233c-19d1-4423-b161-e5256766be8e/SSD/trafficlight/shanzhen/crop/2/try2/0424') as f:
    lines=f.readlines()
for line in lines:
    index=find_last(line,'/')
    name=line[index+1:-5]
    savetxt.write('img0424/'+name+'.jpg'+' '+'xml0424/'+name+'.xml'+'\n')
