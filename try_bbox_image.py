import cv2
import cv2
import numpy as np
import os 
from matplotlib import pyplot as plt



image_path = '/media/pengshanzhen/bb42233c-19d1-4423-b161-e5256766be8e/SSD/trafficlight/shanzhen/crop/2/img/'
label_path = '/media/pengshanzhen/bb42233c-19d1-4423-b161-e5256766be8e/SSD/trafficlight/shanzhen/crop/2/label/'


test_img_path = '/media/pengshanzhen/bb42233c-19d1-4423-b161-e5256766be8e/SSD/trafficlight/shanzhen/crop/2/test_img'


filelist = os.listdir(label_path)
for fname in filelist:
  

  every_label_path = os.path.join(label_path,fname)
  every_image_path = os.path.join(image_path,fname.split('.')[0]+'.jpg')
  every_test_image_path = os.path.join(test_img_path,'result_'+fname.split('.')[0]+'.jpg')
  image = cv2.imread(every_image_path)
  #print(image)
  #exit()
  
  f = open(every_label_path, 'r')
  piexl_list =[]
  for line in f.readlines():
    #every_line =line.strip()
    count = len(line) 

    if count ==2:
      continue
    
    line = line.strip()
    x1 = line.split(' ')[1]
    x1 = int(x1)
    piexl_list.append(x1)
    x2 =line.split(' ')[2] 
    x2 = int(x2)
    piexl_list.append(x2)
    x3 =line.split(' ')[3] 
    x3 = int(x3)
    piexl_list.append(x3)
    x4 =line.split(' ')[4]
    x4 = int(x4) 
    piexl_list.append(x4)
    
    cv2.rectangle(image,(x1,x2),(x3,x4),(55,255,155),1)
    
  



  
    


  
  





#img = np.zeros((512,512,3),np.uint8)
#cv2.rectangle(img,(20,20),(411,411),(55,255,155),5)
  cv2.imwrite(every_test_image_path, image)
  




















