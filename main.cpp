#include <iostream>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <caffe/caffe.hpp>
#include <string>
#include <fstream>

using namespace std;
using namespace cv;
using namespace caffe;

struct object
{
    float score;
    int id;
    int x_min;
    int y_min;
    int x_max;
    int y_max;
    int area;
};

/*void init()
{
    Caffe::set_mode(Caffe::GPU);

    net_.reset(new caffe::Net<float>("/media/d/work/weizhang/tmpssd/net/deploy.prototxt",TEST));
    net_->CopyTrainedLayersFrom("/media/d/work/weizhang/tmpssd/x/model_iter_1000.caffemodel");
    input_layer=net_->input_blobs()[0];
    num_channels = input_layer->channels();
    CHECK(num_channels == 3 || num_channels == 1)
        << "Input layer should have 1 or 3 channels.";
}*/
void dect(cv::Mat &img, vector<object> &obj, float thresh,caffe::shared_ptr<caffe::Net<float> > net_)
{

    caffe::Blob<float> *input_layer=net_->input_blobs()[0];
    int num_channels = input_layer->channels();
    CHECK(num_channels == 3 || num_channels == 1)
        << "Input layer should have 1 or 3 channels.";
    cv::Scalar mean=cv::Scalar(104,117,123);
    //cv::Mat img=cv::imread("/media/d/work/weizhang/trafficlight/testcrop/163_4_0.jpg");
    //cv::Mat img=cv::imread("/media/d/work/SSD/caffe/data/dataset_train_rgb/JPEGImages/31294.jpg");
    cv::Mat sample,sample_single;

    if (img.channels() == 3 && num_channels == 1)
        cv::cvtColor(img, sample, cv::COLOR_BGR2GRAY);
    else if (img.channels() == 4 && num_channels == 1)
        cv::cvtColor(img, sample, cv::COLOR_BGRA2GRAY);
    else if (img.channels() == 4 && num_channels == 3)
        cv::cvtColor(img, sample, cv::COLOR_BGRA2BGR);
    else if (img.channels() == 1 && num_channels == 3)
        cv::cvtColor(img, sample, cv::COLOR_GRAY2BGR);
    else
        sample = img.clone();
    cv::resize(sample,sample,cv::Size(input_layer->width(),input_layer->height()));
    cv::Mat sample_single1;
    sample.convertTo(sample_single1,CV_32FC3);
    //sample_single=sample_single1-cv::Scalar(127.5, 127.5, 127.5);
    //sample_single = sample_single * 0.0078125;
    //sample_single = sample_single1 /127.5-1;
    sample_single =sample_single1-mean;
    //sample_single=sample_single1/127.5-1;
    float *input_data=NULL;
    input_data=input_layer->mutable_cpu_data();
    std::vector<cv::Mat>* input_channels = new std::vector<cv::Mat>;
    for(int i=0;i<input_layer->channels();i++)
    {
        cv::Mat channel(input_layer->height(),input_layer->width(),CV_32FC1,input_data);
        input_channels->push_back(channel);
        input_data +=(input_layer->height())*(input_layer->width());
    }
    cv::split(sample_single,*input_channels);

    net_->Forward();
    Blob<float>* result_blob = net_->output_blobs()[0];
    const float* result = result_blob->cpu_data();
    const int num_det = result_blob->height();
    std::vector<std::vector<float> > detections;
    //std::cout<<num_det<<std::endl;
    for (int k = 0; k < num_det; ++k) {
        if (result[0] == -1) {
            result += 7;
            continue;
        }
        std::vector<float> detection(result, result + 7);
        detections.push_back(detection);
        result += 7;
    }
    //std::cout<<detections.size()<<std::endl;
    for(int i=0;i<detections.size();i++)
    {
        const vector<float>& d = detections[i];
        const float score = d[2];
        //cout<<d[1]<<endl;
        std::string strscore;
        stringstream instr;
        instr<<score;
        instr>>strscore;
        std::string strlabel;
        stringstream instr1;
        instr1<<d[1];
        instr1>>strlabel;
        if(score >= thresh)
        {
            object newobj;
            int xmin = static_cast<int>(d[3] * img.cols);
            int ymin = static_cast<int>(d[4] * img.rows);
            int xmax = static_cast<int>(d[5] * img.cols);
            int ymax = static_cast<int>(d[6] * img.rows);
            //cv::rectangle(img,cv::Rect(xmin,ymin,xmax-xmin,ymax-ymin),cv::Scalar(0,0,255),2,8);
            //cv::putText(img,strlabel+':'+strscore,cv::Point(xmin,ymin),cv::FONT_HERSHEY_COMPLEX,1,cv::Scalar(255,0,0),1);
            newobj.score=d[2];
            newobj.id=d[1];
            newobj.x_min=xmin;
            newobj.y_min=ymin;
            newobj.x_max=xmax;
            newobj.y_max=ymax;
            obj.push_back(newobj);
        }
    }
    //cv::imshow("img",img);
    //cv::waitKey(0);
}
bool comp(const object &a,const object &b)
{
    return a.area>b.area;
}
void process(cv::Mat &image, std::vector<object> &outobjects,  float thresh,caffe::shared_ptr<caffe::Net<float> > net_)
{
    std::vector<object> objects;
    cv::Mat img[3];
    img[0]=image(cv::Rect(0,0,image.cols/2,image.rows/2-50));
    img[1]=image(cv::Rect(image.cols/2,0,image.cols/2,image.rows/2-50));
    img[2]=image(cv::Rect(0,image.rows/2-50,image.cols/2,image.rows/2-50));

    {
        std::vector<object> object1;
        cv::Mat src=image.clone();
        int w=src.cols;
        int h=src.rows;
        int cropw=300;
        int croph=300;
        int MaxlightSize=150;
        int num_w=(w-MaxlightSize)/(cropw-MaxlightSize)+1;
        int num_h=(h-MaxlightSize)/(croph-MaxlightSize)+1;
        for(int index_i=0;index_i<num_w;index_i++)
        {
            for(int index_j=0;index_j<num_h/2;index_j++)
            {
                cv::Rect imgrect(min(index_i*(cropw-MaxlightSize),w-cropw),min(index_j*(croph-MaxlightSize),h-croph),cropw,croph);
                cv::Mat cropedimg=src(imgrect);
                vector<object> cropobj;
                dect(cropedimg,cropobj,thresh,net_);
                //cout<<"objnums:"<<cropobj.size()<<endl;
                for(int index=0;index<cropobj.size();index++)
                {
                    cropobj[index].x_min+=imgrect.x;
                    cropobj[index].y_min+=imgrect.y;
                    cropobj[index].x_max+=imgrect.x;
                    cropobj[index].y_max+=imgrect.y;
                    object1.push_back(cropobj[index]);
                }

            }
        }
        for(int index=0;index<object1.size();index++)
        {

                objects.push_back(object1[index]);
        }
    }
    if(objects.size()>1)
    {
        for(int i=0;i<objects.size()-1;i++)
        {
            if(objects[i].id==50)
            {
                continue;
            }
            for(int j=i+1;j<objects.size();j++)
            {
                cv::Rect r1(objects[i].x_min,objects[i].y_min,objects[i].x_max-objects[i].x_min,objects[i].y_max-objects[i].y_min);
                cv::Rect r2(objects[j].x_min,objects[j].y_min,objects[j].x_max-objects[j].x_min,objects[j].y_max-objects[j].y_min);
                if(float(float((r1&r2).area())/float((r1|r2).area()))>0.8)
                {
                    objects[i].x_min=(objects[i].x_min+objects[j].x_min)/2;
                    objects[i].y_min=(objects[i].y_min+objects[j].y_min)/2;
                    objects[i].x_max=(objects[i].x_max+objects[j].x_max)/2;
                    objects[i].y_max=(objects[i].y_max+objects[j].y_max)/2;
                    //objects[i].area=(objects[i].x_max-objects[i].x_min)*(objects[i].y_max-objects[i].y_min);
                    objects[j].id=50;
                }
            }
        }
    }
    vector<object>::iterator it = objects.begin();
    for(;it!=objects.end();)
    {
        if(it->id ==50)
        {
           it = objects.erase(it);
        }
        else
        {
            it++;
        }
    }
    for(int i=0;i<objects.size();i++)
    {
       objects[i].area=(objects[i].x_max-objects[i].x_min)*(objects[i].y_max-objects[i].y_min);
    }
    sort(objects.begin(),objects.end(),comp);
    for(int i=0;i<objects.size();i++)
    {
        cout<<objects[i].area<<" "<<objects[i].x_min<<" "<<objects[i].x_max<<" "<<objects[i].y_min<<" "<<objects[i].y_max<<endl;
        outobjects.push_back(objects[i]);
    }
}

int main()
{
    Caffe::set_mode(Caffe::GPU);
    caffe::shared_ptr<caffe::Net<float> >net_;
    //net_.reset(new caffe::Net<float>("/media/d/work/weizhang/tmpssd/net/deploy.prototxt",TEST));
    net_.reset(new caffe::Net<float>("/media/pengshanzhen/bb42233c-19d1-4423-b161-e5256766be8e/SSD/trafficlight/shanzhen/crop/2/net/deploysmall.prototxt",TEST));
    //net_->CopyTrainedLayersFrom("/media/d/work/weizhang/tmpssd/x/model_iter_661000.caffemodel");
    net_->CopyTrainedLayersFrom("/media/pengshanzhen/bb42233c-19d1-4423-b161-e5256766be8e/SSD/trafficlight/shanzhen/crop/2/try0426/model/model_iter_55000.caffemodel");



   // string imgpath="/media/d/work/weizhang/run_red_light_croped.txt";
    std::ifstream infile("/media/pengshanzhen/bb42233c-19d1-4423-b161-e5256766be8e/1_xuzhneghao/run_test.txt");
    //std::ifstream infile("/media/d/work/weizhang/run_red_light_croped.txt");
    //std::ifstream infile("/media/d/work/weizhang/data/0423闯红灯_无需切分/noneedcrop");
    string line;
    int num=0;
    while(getline(infile,line))
    {
        num++;
        cout<<"num:"<<num<<endl;
        if(num<2000)
        {
            //continue;
        }
    cv::Mat img=cv::imread(line);
        cv::Mat saveimg=img.clone();
    int index=line.find_last_of("/");
    string name=line.substr(index);
    vector<object> obj;
    //tra.ins();
    process(img,obj,0.5,net_);
    //cout<<"objnums:"<<obj.size()<<endl;
    /*if(obj.size()==0)
    {
        cv::imwrite("/media/d/work/weizhang/data/0423闯红灯_无需切分/0423noneedcrop/"+name,img);
    }*/
    for(int i=0;i<obj.size();i++)
    {
        int xmin=obj[i].x_min;
        int ymin=obj[i].y_min;
        int xmax=obj[i].x_max;
        int ymax=obj[i].y_max;
        int id=obj[i].id;
        cout<<id<<endl;
        if(id==1||id==3||id==7||id==18||id==19||id==20)
        {
            cv::rectangle(img,cv::Rect(xmin,ymin,xmax-xmin,ymax-ymin),cv::Scalar(0,0,255),2,8);
        }
        if(id==6||id==9)
        {
            cv::rectangle(img,cv::Rect(xmin,ymin,xmax-xmin,ymax-ymin),cv::Scalar(255,255,255),2,8);
        }
        if(id==4||id==13||id==14||id==15||id==16||id==17)
        {
            cv::rectangle(img,cv::Rect(xmin,ymin,xmax-xmin,ymax-ymin),cv::Scalar(0,255,255),2,8);
        }
        if(id==2||id==5||id==8||id==10||id==11||id==12)
        {
            cv::rectangle(img,cv::Rect(xmin,ymin,xmax-xmin,ymax-ymin),cv::Scalar(0,255,0),2,8);
        }
        const float score = obj[i].score;
        //cout<<d[1]<<endl;
        std::string strscore;
        stringstream instr;
        instr<<score;
        instr>>strscore;
        std::string strlabel;
        stringstream instr1;

        instr1<<obj[i].id;
        instr1>>strlabel;

         cv::putText(img,strlabel+':'+strscore,cv::Point(xmin,ymin),cv::FONT_HERSHEY_COMPLEX,1,cv::Scalar(255,0,0),1);


    }
    cv::Mat showimg;
    cv::resize(img,showimg,img.size());
    cv::namedWindow("out",1);
    cv::imshow("out",showimg);
        if(cv::waitKey(0)==27)
        {
            cv::imwrite("/media/pengshanzhen/bb42233c-19d1-4423-b161-e5256766be8e/1_xuzhneghao/run_red_light_test/"+name,saveimg);
        }

    }
    return 0;


}
