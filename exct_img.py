import os
import cv2



image_path ='/media/pengshanzhen/bb42233c-19d1-4423-b161-e5256766be8e/SSD/trafficlight/shanzhen/crop/2/try0426/0425img/'
xml_path = '/media/pengshanzhen/bb42233c-19d1-4423-b161-e5256766be8e/SSD/trafficlight/shanzhen/crop/2/try0426/0425xml'

out_image_path = '/media/pengshanzhen/bb42233c-19d1-4423-b161-e5256766be8e/SSD/trafficlight/shanzhen/crop/2/try0426/out_0425img/'


filelist = os.listdir(xml_path)
for fname in filelist:
  every_image_path = os.path.join(image_path,fname.split('.')[0]+'.jpg')
  img = cv2.imread(every_image_path)
  out_every_image_path =  os.path.join(out_image_path,fname.split('.')[0]+'.jpg')
  cv2.imwrite(out_every_image_path,img)
  

