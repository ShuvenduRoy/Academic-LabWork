#include <opencv2\opencv.hpp>
#include <stdio.h>
#include <iostream>

using namespace std;
using namespace cv;

Mat src, dst;

void load_and_show_image()
{
	src = imread("lena.jpg");

	imshow("lena", src);
	waitKey(0);

	Mat grayLena;
	cvtColor(src, grayLena, CV_BGR2GRAY);

	imshow("lenaGray", grayLena);

	waitKey(0);
}

void im_show() {
	Mat img = imread("lena.jpg");
	if (img.empty()) return;

	//namedWindow("example1", WINDOW_AUTOSIZE);
	imshow("example1", img);
	waitKey(0);

	//destroyWindow("example1");
	
}

void simpleTransform()
{
	Mat img = imread("lena.jpg");

	imshow("lena", img);

	Mat out;
	GaussianBlur(img, out, Size(5, 5), 3, 3);

	imshow("out", out);
}


void image_info(Mat img)
{
	//Mat img = imread("lena.jpg");
	cout << "total " << img.total() << endl;
	cout << "type "<<img.type() << endl;
	cout << "size "<<img.size() << endl;
	cout << "channels "<<img.channels() << endl;
	cout << "depth "<<img.depth() << endl<<endl;
}

void _gamma_transform(Mat img, String name, double c, double gamma)
{
	Mat gm = img.clone();
	int row = gm.size[0];
	int col = gm.size[1];

	for (int i = 0; i<row; i++)
	{
		for (int j = 0; j<col; j++)
		{
			/*if (gamma < 1) {
				gm.at<uchar>(i, j) = uchar(c * pow(gm.at<uchar>(i, j), gamma));
			}
			else {
				gm.at<uchar>(i, j) = uchar((c * pow(gm.at<uchar>(i, j), gamma))/pow(255, gamma-1));
			}*/

			gm.at<uchar>(i, j) = uchar((c * pow(gm.at<uchar>(i, j), gamma)) / pow(255, gamma - 1));
			
		}
	}

	imshow(name, gm);
	waitKey(0);
}

void gamma_transform()
{
	Mat img = imread("lena.jpg");
	image_info(img);
	imshow("lena", img);

	cvtColor(img, img, CV_BGR2GRAY);
	imshow("gray original", img);

	// Values of c and gamma
	double c = 1;
	double gamma = 0.5;

	_gamma_transform(img,"0.5", 1, 0.5);
	_gamma_transform(img,"0.9", 1, 0.9);

	/*_gamma_transform(img, "10, 0.5", 10, 0.5);
	_gamma_transform(img, "10 0.9", 10, 0.9);*/

	_gamma_transform(img, "1 2.0", 1, 2.0);
	_gamma_transform(img, "1 10.0", 1, 10.0);
	
	
}

void intensity_slicing() {
	Mat img = imread("lena.jpg");
	image_info(img);
	cvtColor(img, img, CV_BGR2GRAY);
	
	imshow("lena", img);

	Mat des = img.clone();
	int x1 = 90, y1 = 70, x2 = 200, y2 = 220;
	cin>>x1>>y1>>x2>>y2;

	int row = img.size[0];
	int col = img.size[1];

	for (int i = 0; i < row; i++) {
		for (int j = 0; j < col; j++) {
			double slope = (double)y1 / x1;
			double slope1 = (double)(y2 - y1) / (x2 - x1);
			double slope2 = (double)(255 - y2) / (255 - x2);

			if (img.at<uchar>(i, j) <= x1) {
				des.at<uchar>(i, j) = (uchar)(slope * (double)img.at<uchar>(i, j));
			}
			else if (img.at<uchar>(i, j) <= x2) {
				des.at<uchar>(i, j) = (uchar)(slope1 * (double)img.at<uchar>(i, j));
			}
			else {
				des.at<uchar>(i, j) = (uchar)(slope2 * (double)img.at<uchar>(i, j));
			}
		}
	}

	imshow("intensity shifted image", des);

}


void spatial_filtering()
{
	Mat img = imread("lena.jpg");
	image_info(img);
	cvtColor(img, img, CV_BGR2GRAY);
	
	imshow("lena", img);

	int row = img.size[0];
	int col = img.size[1];

	cout<<"Enter the value of sigma: ";
	float sigma;
	cin>>sigma;

	// Calculating kernel and padding size
	int kernel_size = ceil(sigma * 5);
	if(kernel_size % 2 == 0 ){
		kernel_size = kernel_size + 1;
	}

	int padding_size = kernel_size/2;
	cout<<kernel_size<< " "<<padding_size<<endl;
	
	// Creating destination image
	Mat des;
	des.create( (row - padding_size*2), (col - padding_size*2), CV_8U);
	image_info(des);

	// Creating kernel
	Mat kernel;
	kernel.create(kernel_size, kernel_size, CV_32FC1);
	image_info(kernel);


	// Calculating kernel
	for(int i=-padding_size; i<=padding_size; i++)
	{
		for(int j=-padding_size; j<=padding_size; j++){
			float val = exp( -(pow(i, 2) + pow(j, 2)) / 2 * (pow(sigma, 2)) );
			kernel.at<float>(i+padding_size, j+padding_size) = val;
			//cout<<val<<endl;
		}
	}

	// printing kernel for test
	cout<<"kernel before normalization"<<endl;
	for(int i=-padding_size; i<=padding_size; i++)
	{
		for(int j=-padding_size; j<=padding_size; j++){
			cout<<kernel.at<float>(i+padding_size, j+padding_size)<<" ";

		}
		cout<<endl;
	}


	// Normalize the kernel
	float norm = kernel.at<float>(0, 0);
	float total = 0;
	for(int i=-padding_size; i<=padding_size; i++)
	{
		for(int j=-padding_size; j<=padding_size; j++){
			float val = ceil(kernel.at<float>(i+padding_size, j+padding_size) / norm);
			kernel.at<float>(i+padding_size, j+padding_size) = val;
			total += val;
		}
		cout<<endl;
	}

	cout<<"Normalization factor : "<<total<<endl;



	// printing kernel for test
	cout<<"kernel after normalization"<<endl;
	for(int i=-padding_size; i<=padding_size; i++)
	{
		for(int j=-padding_size; j<=padding_size; j++){
			cout<<kernel.at<float>(i+padding_size, j+padding_size)<<" ";

		}
		cout<<endl;
	}


	// Convolutional operation
	for(int i=0; i<row- 2 * padding_size; i++){
		for(int j=0; j<col - 2*padding_size; j++){
			//position of image to start kernel calculation

			float ans=0;
			for(int a=0; a<kernel_size; a++){
				for(int b=0; b<kernel_size; b++){
					ans += img.at<uchar>(i+a, j+b) * kernel.at<float>(a, b);
				}
			}

			ans = (uchar)(ans / total);
			des.at<uchar>(i,j) = ans;


		} 
	}

	imshow("gausaian filtered", des);

	Mat ocvGblur;
	GaussianBlur(img,ocvGblur,Size(7,7),1.4,1.4);
	imshow("OCV_G_Blur",ocvGblur);


	
}


void sharpen()
{
	Mat lena, inp, dest, kernel;
	lena = imread("lena.jpg");
	//image_info(lena);

	int row, col,padding_size;
	float sigma=float(1.4),total=0;
	//cout << "Enter the value of sigma: ";
	//cin >> sigma;

	row = lena.size[0];
	col = lena.size[1];

	cvtColor(lena, inp, CV_BGR2GRAY);
	imshow("Gray", inp);
	waitKey(0);

	int kernel_size = int(ceil(sigma * 5));
	if (kernel_size % 2 == 0) {
		kernel_size++;
	}
	padding_size = kernel_size / 2;

	//cout << kernel_size << endl;

	//Creating Kernel 
	kernel.create(kernel_size, kernel_size, CV_32FC1);
	for (int i = -padding_size; i <= padding_size; i++) {
		for (int j = -padding_size; j <= padding_size; j++) {
			float val = float(exp((pow(i, 2) + pow(j, 2)) / (2 * pow(sigma, 2))));
			kernel.at<float>(i + padding_size, j + padding_size)=val;
			total += val;
		}
	}
	//convolution over the loop 
	dest.create(row,col,CV_8U);

	for (int i = padding_size; i < row - 2 * padding_size; i++) {
		for (int j = padding_size; j < col - 2 * padding_size; j++) {
			float sum = 0.0;
			for (int a = 0; a < kernel_size; a++) {
				for (int b = 0; b < kernel_size; b++) {
					sum += inp.at<uchar>(i + a, j + b)*kernel.at<float>(a, b);
				}
			}
			dest.at<uchar>(i, j) = int(ceil(sum / total));
		}
	}

	imshow("Out", dest);
	waitKey(0);
	//highboost filtering and unsharp masking
	image_info(dest);
	Mat mask;
	mask.create(row, col, CV_8UC1);

	mask = inp - dest;

	imshow("Mask", mask);
	waitKey(0);

	float k = 1;
	Mat g = inp + (k*mask);

	imshow("Unsharp", g);
	waitKey(0);

}


void erosion()
{
	Mat img = imread("star.tif");
	cvtColor(img, img, CV_RGB2GRAY);
	int kernel_size = 15;

	copyMakeBorder(img, img, kernel_size/2,kernel_size/2,kernel_size/2,kernel_size/2,0,0);
	//imshow("Star", img);

	Mat des = img.clone();
	image_info(des);

	int row = des.size[0];
	int col = des.size[1];
	cout<<"Row "<<row<<" Col "<<col<<endl;

	for (int i = 0; i < row; i++) {
		for (int j = 0; j < col; j++) {
			des.at<uchar>(i, j) = 255;
		}
	}
	imshow("Src", img);

	

	Mat kernel;
	kernel.create(kernel_size, kernel_size, CV_8U);
	for(int i=0; i<kernel_size; i++){
		for(int j=0; j<kernel_size; j++){
			kernel.at<uchar>(i, j) = 255;
		}
	}

	des.convertTo(des, CV_8U);
	cout<<(des.type())<<endl;

	for (int i = 0; i < row-kernel_size-1; i++) {
		for (int j = 0; j < col-kernel_size-1; j++) {
			
			bool matched = true;

			for(int x=0; x<kernel_size; x++){
				for(int y=0; y<kernel_size; y++){
					if(img.at<uchar>(i+x,j+y) != kernel.at<uchar>(x, y)){
						matched = false;
					}
				}
			}

			if(matched){
				des.at<uchar>(i+kernel_size/2, j+kernel_size/2) = 255;
			} else {
				des.at<uchar>(i+kernel_size/2, j+kernel_size/2) = 0;
			}
		}
	}

	imshow("Final image", des);
}


void edge_detector(){
	Mat img = imread("man.tif");
	cvtColor(img, img, CV_RGB2GRAY);
	int kernel_size = 3;

	copyMakeBorder(img, img, kernel_size/2,kernel_size/2,kernel_size/2,kernel_size/2,0,0);
	//imshow("Star", img);

	Mat des = img.clone();
	image_info(des);

	int row = des.size[0];
	int col = des.size[1];
	cout<<"Row "<<row<<" Col "<<col<<endl;

	for (int i = 0; i < row; i++) {
		for (int j = 0; j < col; j++) {
			des.at<uchar>(i, j) = 255;
		}
	}
	imshow("Src", img);

	

	Mat kernel;
	kernel.create(kernel_size, kernel_size, CV_8U);
	for(int i=0; i<kernel_size; i++){
		for(int j=0; j<kernel_size; j++){
			kernel.at<uchar>(i, j) = 255;
		}
	}

	des.convertTo(des, CV_8U);
	cout<<(des.type())<<endl;

	for (int i = 0; i < row-kernel_size-1; i++) {
		for (int j = 0; j < col-kernel_size-1; j++) {
			
			bool matched = true;

			for(int x=0; x<kernel_size; x++){
				for(int y=0; y<kernel_size; y++){
					if(img.at<uchar>(i+x,j+y) != kernel.at<uchar>(x, y)){
						matched = false;
					}
				}
			}

			if(matched){
				des.at<uchar>(i+kernel_size/2, j+kernel_size/2) = 255;
			} else {
				des.at<uchar>(i+kernel_size/2, j+kernel_size/2) = 0;
			}
		}
	}

	imshow("Final image", des);

	Mat edge = img - des;
	imshow("Edge", edge);
}

void dilation(){
	Mat img = imread("finger_print.tif");
	cvtColor(img, img, CV_RGB2GRAY);
	int kernel_size = 3;

	copyMakeBorder(img, img, kernel_size/2,kernel_size/2,kernel_size/2,kernel_size/2,0,0);
	//imshow("Star", img);

	Mat des = img.clone();
	image_info(des);

	int row = des.size[0];
	int col = des.size[1];
	cout<<"Row "<<row<<" Col "<<col<<endl;

	for (int i = 0; i < row; i++) {
		for (int j = 0; j < col; j++) {
			des.at<uchar>(i, j) = 255;
		}
	}
	imshow("Src", img);

	

	Mat kernel;
	kernel.create(kernel_size, kernel_size, CV_8U);
	for(int i=0; i<kernel_size; i++){
		for(int j=0; j<kernel_size; j++){
			kernel.at<uchar>(i, j) = 255;
		}
	}

	des.convertTo(des, CV_8U);
	cout<<(des.type())<<endl;

	for (int i = 0; i < row-kernel_size-1; i++) {
		for (int j = 0; j < col-kernel_size-1; j++) {
			
			bool matched = false;

			for(int x=0; x<kernel_size; x++){
				for(int y=0; y<kernel_size; y++){
					if(img.at<uchar>(i+x,j+y) == kernel.at<uchar>(x, y)){
						matched = true;
					}
				}
			}

			if(matched){
				des.at<uchar>(i+kernel_size/2, j+kernel_size/2) = 255;
			} else {
				des.at<uchar>(i+kernel_size/2, j+kernel_size/2) = 0;
			}

		}
	}

	imshow("Final image", des);
}


void finger_print(){
	Mat img = imread("finger_print.tif");
	cvtColor(img, img, CV_RGB2GRAY);
	int kernel_size = 3;

	copyMakeBorder(img, img, kernel_size/2,kernel_size/2,kernel_size/2,kernel_size/2,0,0);
	//imshow("Star", img);

	Mat des = img.clone();
	image_info(des);

	int row = des.size[0];
	int col = des.size[1];
	cout<<"Row "<<row<<" Col "<<col<<endl;

	for (int i = 0; i < row; i++) {
		for (int j = 0; j < col; j++) {
			des.at<uchar>(i, j) = 255;
		}
	}
	imshow("Src", img);

	

	Mat kernel;
	kernel.create(kernel_size, kernel_size, CV_8U);
	for(int i=0; i<kernel_size; i++){
		for(int j=0; j<kernel_size; j++){
			kernel.at<uchar>(i, j) = 255;
		}
	}

	des.convertTo(des, CV_8U);
	cout<<(des.type())<<endl;

	for (int i = 0; i < row-kernel_size-1; i++) {
		for (int j = 0; j < col-kernel_size-1; j++) {
			
			bool matched = true;

			for(int x=0; x<kernel_size; x++){
				for(int y=0; y<kernel_size; y++){
					if(img.at<uchar>(i+x,j+y) != kernel.at<uchar>(x, y)){
						matched = false;
					}
				}
			}

			if(matched){
				des.at<uchar>(i+kernel_size/2, j+kernel_size/2) = 255;
			} else {
				des.at<uchar>(i+kernel_size/2, j+kernel_size/2) = 0;
			}
		}
	}

	imshow("corotion image", des);
	img = des;
	//imshow("new src", img);

	des = img.clone();
	//imshow("new des", des);

	for (int i = 0; i < row-kernel_size-1; i++) {
		for (int j = 0; j < col-kernel_size-1; j++) {
			
			bool matched = false;

			for(int x=0; x<kernel_size; x++){
				for(int y=0; y<kernel_size; y++){
					if(img.at<uchar>(i+x,j+y) == kernel.at<uchar>(x, y)){
						matched = true;
					}
				}
			}

			if(matched){
				des.at<uchar>(i+kernel_size/2, j+kernel_size/2) = 255;
			} else {
				des.at<uchar>(i+kernel_size/2, j+kernel_size/2) = 0;
			}

		}
	}
	imshow("Final image Step 1", des);

	img = des;
	des = img.clone();

	for (int i = 0; i < row-kernel_size-1; i++) {
		for (int j = 0; j < col-kernel_size-1; j++) {
			
			bool matched = true;

			for(int x=0; x<kernel_size; x++){
				for(int y=0; y<kernel_size; y++){
					if(img.at<uchar>(i+x,j+y) != kernel.at<uchar>(x, y)){
						matched = false;
					}
				}
			}

			if(matched){
				des.at<uchar>(i+kernel_size/2, j+kernel_size/2) = 255;
			} else {
				des.at<uchar>(i+kernel_size/2, j+kernel_size/2) = 0;
			}
		}
	}

	imshow("Final image Step 2", des);
}


void show_hist(Mat img, String id)
{
	
	//image_info(img);
	//cvtColor(img, img, CV_BGR2GRAY);
	/*imshow("lena", img);*/

	int row = img.size[0];
	int col = img.size[1];

	//Declearing local variables
	int intensity_count[256]; 
	Mat freq = Mat::zeros(1,256,CV_32SC1);
	int *pt_freq=freq.ptr<int>(0);

	float normalized_intensity[256];
	float cumulative_prob[256];

	for(int i=0; i<256; i++){
		intensity_count[i] = 0;
	}

	/*for(int i=0; i<row; i++){
		for(int j=0; j<col; j++){
			int intensity = img.at<uchar>(i, j);
			intensity_count[intensity] += 1;
			pt_freq[intensity]++;
		}
	}*/

	
	for(int i=0; i<row; i++){
		uchar *pt = img.ptr<uchar>(i);
		for(int j=0; j<col; j++){
			int intensity = pt[j]; //img.at<uchar>(i, j);
			intensity_count[intensity] += 1;
			pt_freq[intensity]++;
		}
	}


	float max_intensity = 0.0;
	int mn = row*col;

	freq /= mn;


	for(int i=0; i<256; i++){
		normalized_intensity[i] = (float)intensity_count[i]/mn;
		if (normalized_intensity[i] > max_intensity){
			max_intensity = normalized_intensity[i];
		}
	}

	
	cumulative_prob[0] = normalized_intensity[0];
	for(int i=1; i<256; i++){
		cumulative_prob[i] = cumulative_prob[i-1] + normalized_intensity[i];
		//cout<<cumulative_prob[i]<<endl;
	}

	Mat hist;
	hist.create(300, 256, CV_8UC1);
	hist.setTo(256);
	

	for(int j=0; j<256; j++){
		int height = 300 * normalized_intensity[j] / max_intensity;
		for(int i=300-1; i>(300-height); i--){
			hist.at<uchar>(i, j) = 0;
		}
	}

	imshow("hist"+id, hist);
}

void show_hist_mat(Mat img, String id)
{
	int row = img.size[0];
	int col = img.size[1];

	//Declearing local variables
	int intensity_count[256]; 
	Mat freq = Mat::zeros(1,256,CV_32SC1);
	int *pt_freq=freq.ptr<int>(0);

	float normalized_intensity[256];
	float cumulative_prob[256];

	
	for(int i=0; i<row; i++){
		uchar *pt = img.ptr<uchar>(i);
		for(int j=0; j<col; j++){
			int intensity = pt[j]; //img.at<uchar>(i, j);
			pt_freq[intensity]++;
		}
	}


	float max_intensity = 0.0;
	int mn = row*col;

	Mat normalized_freq;
	freq.convertTo(normalized_freq, CV_32FC1);
	normalized_freq /= mn;

	float *pt_normalized_freq = normalized_freq.ptr<float>(0);



	for(int i=0; i<256; i++){
		if (pt_normalized_freq[i] > max_intensity){
			max_intensity = pt_normalized_freq[i];
		}
	}

	
	cumulative_prob[0] = pt_normalized_freq[0];
	for(int i=1; i<256; i++){
		cumulative_prob[i] = cumulative_prob[i-1] + pt_normalized_freq[i];
		//cout<<cumulative_prob[i]<<endl;
	}

	Mat hist;
	hist.create(300, 256, CV_8UC1);
	hist.setTo(256);
	

	for(int j=0; j<256; j++){
		int height = 300 * pt_normalized_freq[j] / max_intensity;
		for(int i=300-1; i>(300-height); i--){
			hist.at<uchar>(i, j) = 0;
		}
	}

	imshow("hist"+id, hist);
}


void histogram_equalization(){
	Mat img = imread("Fig0310.tif");
	show_hist_mat(img, "0");

	image_info(img);
	cvtColor(img, img, CV_BGR2GRAY);
	imshow("original image", img);

	int row = img.size[0];
	int col = img.size[1];

	//Declearing local variables
	int intensity_count[256];  
	float normalized_intensity[256];
	float cumulative_prob[256];

	for(int i=0; i<256; i++){
		intensity_count[i] = 0;
	}

	for(int i=0; i<row; i++){
		uchar *pt = img.ptr<uchar>(i);
		for(int j=0; j<col; j++){
			int intensity = pt[j];//img.at<uchar>(i, j);
			intensity_count[intensity] += 1;
			//pt_freq[intensity]++;
		}
	}

	float max_intensity = 0.0;
	int mn = row*col;

	for(int i=0; i<256; i++){
		normalized_intensity[i] = (float)intensity_count[i]/mn;
		if (normalized_intensity[i] > max_intensity){
			max_intensity = normalized_intensity[i];
		}
	}

	
	cumulative_prob[0] = normalized_intensity[0];
	for(int i=1; i<256; i++){
		cumulative_prob[i] = cumulative_prob[i-1] + normalized_intensity[i];
		//cout<<cumulative_prob[i]<<endl;
	}

	int transformed_intensity[256];
	for(int i=0; i<256; i++){
		transformed_intensity[i] = 255 * cumulative_prob[i];
		cout<<transformed_intensity[i]<<endl;
	}

	for(int i=0; i<row; i++){
		for(int j=0; j<col; j++){
			int intensity = img.at<uchar>(i, j);
			img.at<uchar>(i, j) = transformed_intensity[intensity];
		}
	}

	imshow("tranformaed image", img);
	show_hist(img, "1");
}



float* histogram(Mat &img)
{
	
	/*Mat n_k_mat = Mat::zeros(1, 256, CV_32FC1);
	uchar *n_k = n_k_mat.ptr(0);*/

	float n_k[256];
	memset(n_k, 0.0, sizeof(n_k));

	float p_k[256]; 
	memset(p_k, 0.0, sizeof(p_k));

	float cum_p_k[256];
	memset(cum_p_k, 0.0, sizeof(cum_p_k));

	int row, col;
	row = img.size[0];
	col = img.size[1];


	//calculating n_k 

	for (int i = 0; i < row; i++) {
		// accessing image with row pointer
		uchar *ptr = img.ptr(i);

		for (int j = 0; j < col; j++) {
			n_k[ptr[j]]++;
		}
	}

	int mn = row * col;
	double max = 0;
	for (int i = 0; i < 256; i++) {
		p_k[i] = double(n_k[i] / mn);

		if (p_k[i] > max) {
			max = p_k[i];
		}
	}

	//calculating cumilative pk
	for (int i = 0; i < 256; i++) {
		for (int j = 0; j <= i; j++) {
			cum_p_k[i] += p_k[j];
		}
	}

	//creating histogram matrix

	Mat hist;
	hist.create(300, 256, CV_8UC1);
	hist.setTo(0);

	//plotting into histogram
	for (int i = 0; i <256; i++) {
		int line = int(300 * p_k[i] / max);
		for (int j = 299; j >= 300 - line; j--) {
			hist.at<uchar>(j, i) = 255;
		}
	}

	//showing the histogram

	imshow("Histogram", hist);
	waitKey(0);

	return cum_p_k;
}


void histogram_matching()
{
	Mat reference_image, target_image;
	reference_image = imread("lena.jpg"); //Fig0316_a.tif
	target_image = imread("Fig0316_b.tif");
	cvtColor(reference_image, reference_image, CV_RGB2GRAY);
	cvtColor(target_image, target_image, CV_RGB2GRAY);


	float *ref_cum_p_k = histogram(reference_image);
	float *tar_cum_p_k = histogram(target_image);

	float ref_s[256], tar_s[256];

	for (int i = 0; i < 256; i++) {
		ref_s[i] = ref_cum_p_k[i] * 256;
		tar_s[i] = tar_cum_p_k[i] * 256;
	}

	float M_G[256];
	memset(M_G, 0.0, sizeof(M_G));

	//for (int g1 = 0; g1 < 256; g1++) {
	//	for (int g2 = 0; g2 < 256; g2++) {
	//		if (ref_s[g1] >= tar_s[g2]) {
	//			M_G[g1] = g2;
	//		}
	//	}
	//}

	for (int g1 = 0; g1 < 256; g1++) {
		int min = 256;
		for (int g2 = 0; g2 < 256; g2++) {
			if (ref_s[g1] <= tar_s[g2] && tar_s[g2] < min) {
				M_G[g1] = g2;
				min = tar_s[g2];
			}
		}
	}


	for (int i = 0; i < reference_image.rows; i++) {
		uchar* ptr = reference_image.ptr(i);
		for (int j = 0; j < reference_image.cols; j++) {
			ptr[j] = M_G[int(ptr[j])];
			//reference_image.at<uchar>(i, j) = M_G[int(reference_image.at<uchar>(i, j))];
		}
	}

	imshow("Reference Image", reference_image);
	waitKey(0);

	float *ref_cum_p_k_new = histogram(reference_image);

	
}



Mat histogram_mat(Mat &img)
{
	
	Mat n_k_mat = Mat::zeros(1, 256, CV_32FC1);
	float *n_k = n_k_mat.ptr<float>(0);

	Mat p_k_mat = Mat::zeros(1, 256, CV_32FC1);
	float *p_k = n_k_mat.ptr<float>(0);

	Mat cum_p_k_mat = Mat::zeros(1, 256, CV_32FC1);
	float *cum_p_k = n_k_mat.ptr<float>(0);

	//float n_k[256];
	//memset(n_k, 0.0, sizeof(n_k));

	//float p_k[256]; 
	//memset(p_k, 0.0, sizeof(p_k));

	//float cum_p_k[256];
	//memset(cum_p_k, 0.0, sizeof(cum_p_k));

	int row, col;
	row = img.size[0];
	col = img.size[1];


	//calculating n_k 

	for (int i = 0; i < row; i++) {
		// accessing image with row pointer
		uchar *ptr = img.ptr(i);

		for (int j = 0; j < col; j++) {
			n_k[ptr[j]]++;
		}
	}

	int mn = row * col;
	double max = 0;
	for (int i = 0; i < 256; i++) {
		p_k[i] = double(n_k[i] / mn);

		if (p_k[i] > max) {
			max = p_k[i];
		}
	}

	//calculating cumilative pk
	for (int i = 0; i < 256; i++) {
		for (int j = 0; j <= i; j++) {
			cum_p_k[i] += p_k[j];
		}
	}

	//creating histogram matrix

	Mat hist;
	hist.create(300, 256, CV_8UC1);
	hist.setTo(0);

	//plotting into histogram
	for (int i = 0; i <256; i++) {
		int line = int(300 * p_k[i] / max);
		for (int j = 299; j >= 300 - line; j--) {
			hist.at<uchar>(j, i) = 255;
		}
	}

	//showing the histogram

	imshow("Histogram", hist);
	waitKey(0);

	return cum_p_k_mat;
}


void histogram_matching_mat()
{
	Mat reference_image, target_image;
	reference_image = imread("lena.jpg"); //Fig0316_a.tif
	target_image = imread("Fig0316_b.tif");
	cvtColor(reference_image, reference_image, CV_RGB2GRAY);
	cvtColor(target_image, target_image, CV_RGB2GRAY);

	Mat ref_cum_p_k_mat = histogram_mat(reference_image);
	float *ref_cum_p_k = ref_cum_p_k_mat.ptr<float>(0);

	Mat tar_cum_p_k_mat = histogram_mat(target_image);
	float *tar_cum_p_k = tar_cum_p_k_mat.ptr<float>(0);

	float ref_s[256], tar_s[256];

	for (int i = 0; i < 256; i++) {
		ref_s[i] = ref_cum_p_k[i] * 256;
		tar_s[i] = tar_cum_p_k[i] * 256;
	}

	float M_G[256];
	memset(M_G, 0.0, sizeof(M_G));

	//for (int g1 = 0; g1 < 256; g1++) {
	//	for (int g2 = 0; g2 < 256; g2++) {
	//		if (ref_s[g1] >= tar_s[g2]) {
	//			M_G[g1] = g2;
	//		}
	//	}
	//}

	for (int g1 = 0; g1 < 256; g1++) {
		int min = 256;
		for (int g2 = 0; g2 < 256; g2++) {
			if (ref_s[g1] <= tar_s[g2] && tar_s[g2] < min) {
				M_G[g1] = g2;
				min = tar_s[g2];
			}
		}
	}


	for (int i = 0; i < reference_image.rows; i++) {
		uchar* ptr = reference_image.ptr(i);
		for (int j = 0; j < reference_image.cols; j++) {
			ptr[j] = M_G[int(ptr[j])];
			//reference_image.at<uchar>(i, j) = M_G[int(reference_image.at<uchar>(i, j))];
		}
	}

	imshow("Reference Image", reference_image);
	waitKey(0);
}


void shapen()
{
	Mat img = imread("lena.jpg");
	image_info(img);
	cvtColor(img, img, CV_BGR2GRAY);
	
	imshow("lena", img);

	int row = img.size[0];
	int col = img.size[1];

	// defining the kernel
	float kernel_[9] = {-1, -1, -1,
						 -1, 8, -1,
						-1, -1, -1};
	Mat kernel = Mat(3,3,CV_32F, kernel_);

	// Applying the filter
	Mat des;
	filter2D(img, des, CV_32F, kernel, cv::Point(-1,-1));
	imshow("filtered", des);

	//Absolute des
	Mat abs_des = cv::abs(des);
	imshow("laplas absolute", abs_des);

	//Normalization
	Mat norm_des;
	cv::normalize(des, norm_des, 0, 255, NORM_MINMAX, CV_8U);
	imshow("normalized laplas", norm_des);

	//shaped
	Mat sharpen;
	cv::addWeighted(img, 0.5, norm_des, 0.5, 0, sharpen);

	Mat norm_sharpen;
	cv::normalize(sharpen, norm_sharpen, 0, 255, NORM_MINMAX, CV_8U);
	imshow("sharpen", norm_sharpen);


	// Extra work::
	//1
	Mat box_1;
	float box_filter_1[9] = {-1,-2,-1,
							0, 0, 0,
							1, 2, 1};

	Mat box_filter_1_kenel = Mat(3,3,CV_32F, box_filter_1);

	filter2D(norm_des, box_1, CV_32F, box_filter_1_kenel, cv::Point(-1,-1));

	Mat box_1_normalized;
	cv::normalize(box_1, box_1_normalized, 0, 255, NORM_MINMAX, CV_8U);
	imshow("box 1 filtered", box_1_normalized);


	// 2
	Mat box_2;
	float box_filter_2[9] = {-1,0, 1,
							-2, 0, 2,
							-1, 0, 1};

	Mat box_filter_2_kenel = Mat(3,3,CV_32F, box_filter_2);

	filter2D(norm_des, box_2, CV_32F, box_filter_2_kenel, cv::Point(-1,-1));

	Mat box_2_normalized;
	cv::normalize(box_2, box_2_normalized, 0, 255, NORM_MINMAX, CV_8U);
	imshow("box 2 filtered", box_2_normalized);
}

void shiftDFT(Mat& fImage )
{
  	int centerX = fImage.cols / 2;
	int centerY = fImage.rows / 2;

	Mat q1(fImage, Rect(0, 0, centerX, centerY));
	Mat q2(fImage, Rect(centerX, 0, centerX, centerY));
	Mat q3(fImage, Rect(0, centerY, centerX, centerY));
	Mat q4(fImage, Rect(centerX, centerY, centerX, centerY));

	Mat swapMap;

	q1.copyTo(swapMap);
	q4.copyTo(q1);
	swapMap.copyTo(q4);

	q2.copyTo(swapMap);
	q3.copyTo(q2);
	swapMap.copyTo(q3);
}


void showDFT(Mat source, String name)
{
	// Split the real and img parts
	Mat splitArray[2] = { Mat::zeros(source.size(), CV_32F), Mat::zeros(source.size(), CV_32F) };
	split(source, splitArray);

	Mat dftMagnitude;  // x^2 + y^2
	magnitude(splitArray[0], splitArray[1], dftMagnitude);

	dftMagnitude += Scalar::all(1);
	log(dftMagnitude, dftMagnitude);

	normalize(dftMagnitude, dftMagnitude, 0, 1, CV_MINMAX);

	imshow("DFT", dftMagnitude);
	waitKey();

	// re center the output
	int centerX = dftMagnitude.cols / 2;
	int centerY = dftMagnitude.rows / 2;

	Mat q1(dftMagnitude, Rect(0, 0, centerX, centerY));
	Mat q2(dftMagnitude, Rect(centerX, 0, centerX, centerY));
	Mat q3(dftMagnitude, Rect(0, centerY, centerX, centerY));
	Mat q4(dftMagnitude, Rect(centerX, centerY, centerX, centerY));

	Mat swapMap;

	q1.copyTo(swapMap);
	q4.copyTo(q1);
	swapMap.copyTo(q4);

	q2.copyTo(swapMap);
	q3.copyTo(q2);
	swapMap.copyTo(q3);
	imshow(name, dftMagnitude);
	waitKey();

}

Mat createSpectrumMagnitudeDisplay(Mat& complexImg, bool rearrange)
{
    Mat planes[2];

    split(complexImg, planes);
    magnitude(planes[0], planes[1], planes[0]);

    Mat mag = (planes[0]).clone();
    mag += Scalar::all(1);
    log(mag, mag);

    if (rearrange)
    {
        // re-arrange the quaderants
        shiftDFT(mag);
    }

    normalize(mag, mag, 0, 1, CV_MINMAX);

    return mag;

}


void low_pass_filter_(Mat dft, int D)
{
	Mat original = dft.clone();

	Mat filter = Mat(dft.rows, dft.cols, CV_32F);

	Point centre = Point(dft.rows / 2, dft.cols / 2);
	double radius;

	for(int i = 0; i < dft.rows; i++)
	{
		for(int j = 0; j < dft.cols; j++)
		{
			radius = (double) sqrt(pow((i - centre.x), 2.0) + pow((double) (j - centre.y), 2.0));
			if(radius>D){
				filter.at<float>(i,j) = (float)0;
			}else{
				filter.at<float>(i,j) = (float)1; 
			}
						
		}
	}

    Mat toMerge[] = {filter, filter};
	merge(toMerge, 2, dft);

	shiftDFT(dft);
	mulSpectrums(original, dft, dft, 0);
	showDFT(dft, "low pass filter");
	shiftDFT(dft);

	Mat inverse;
	cv::dft(dft, inverse, DFT_INVERSE | DFT_REAL_OUTPUT | DFT_SCALE);
	imshow("low pass output image", inverse);
	waitKey();

}

void high_pass_filter_(Mat dft, int D)
{
	Mat original = dft.clone();

	Mat filter = Mat(dft.rows, dft.cols, CV_32F);

	Point centre = Point(dft.rows / 2, dft.cols / 2);
	double radius;

	for(int i = 0; i < dft.rows; i++)
	{
		for(int j = 0; j < dft.cols; j++)
		{
			radius = (double) sqrt(pow((i - centre.x), 2.0) + pow((double) (j - centre.y), 2.0));
			if(radius>D){
				filter.at<float>(i,j) = (float)1;
			}else{
				filter.at<float>(i,j) = (float)0; 
			}
						
		}
	}

    Mat toMerge[] = {filter, filter};
	merge(toMerge, 2, dft);

	shiftDFT(dft);
	mulSpectrums(original, dft, dft, 0);
	showDFT(dft, "high pass filter");
	shiftDFT(dft);

	Mat inverse;
	cv::dft(dft, inverse, DFT_INVERSE | DFT_REAL_OUTPUT | DFT_SCALE);
	imshow("high pass output image", inverse);
	waitKey();

}

void butter_worth_filter_(Mat dft, int D, int n)
{
	Mat original = dft.clone();

	Mat filter = Mat(dft.rows, dft.cols, CV_32F);

	Point centre = Point(dft.rows / 2, dft.cols / 2);
	double radius;

	for(int i = 0; i < dft.rows; i++)
	{
		for(int j = 0; j < dft.cols; j++)
		{
			radius = (double) sqrt(pow((i - centre.x), 2.0) + pow((double) (j - centre.y), 2.0));
			filter.at<float>(i,j) = (float)
						( 1 / (1 + pow((double) (radius /  D), (double) (2 * n))));
		
						
		}
	}

    Mat toMerge[] = {filter, filter};
	merge(toMerge, 2, dft);

	shiftDFT(dft);
	mulSpectrums(original, dft, dft, 0);
	showDFT(dft, "BW filter");
	shiftDFT(dft);

	Mat inverse;
	cv::dft(dft, inverse, DFT_INVERSE | DFT_REAL_OUTPUT | DFT_SCALE);
	imshow("Butter worth output image", inverse);
	waitKey();

}


void fourier_transform_()
{
	// Read image from file
	// Make sure that the image is in grayscale
	Mat original = imread("lena.JPG", CV_LOAD_IMAGE_GRAYSCALE);
	
	// rescale image between 0 & 1
	Mat originalFloat;
	original.convertTo(originalFloat, CV_32FC1, 1.0 / 255.0);

	// we need an object that can hold the real and imaginary part
	Mat originalComplex[2] = { originalFloat, Mat::zeros(originalFloat.size(), CV_32F) };

	//marge this two
	Mat dftReady;
	merge(originalComplex, 2, dftReady);

	// now we can get the dft
	Mat dftOriginal;
	dft(dftReady, dftOriginal, DFT_COMPLEX_OUTPUT);
	Mat dftOriginalClone = dftOriginal.clone();

	showDFT(dftOriginal.clone(), "original dft");

	// inverse fourier transform
	Mat inverse;
	dft(dftOriginal, inverse, DFT_INVERSE | DFT_REAL_OUTPUT | DFT_SCALE);
	imshow("inversed image", inverse);
	waitKey();

	// low pass filter output
	int r = 30;
	low_pass_filter_(dftOriginalClone.clone(), r);
	high_pass_filter_(dftOriginalClone.clone(), r);
	butter_worth_filter_(dftOriginalClone.clone(), r, 2);

}

void binary_thresholding(Mat img, int t, String name)
{

	for(int i=0; i<img.rows; i++){
		uchar *pt = img.ptr<uchar>(i);
		for(int j=0; j<img.cols; j++){
			int intensity = pt[j]; //img.at<uchar>(i, j);
			
			if(intensity > t){
				pt[j] = 255;
			} else {
				pt[j] = 0;
			}
		}
	}

	imshow("binary thresholded"+name, img);
	waitKey();
}

int find_global_threshold(Mat img)
{
	Mat mask = img.clone();
	for(int i=0; i<mask.rows; i++){
		uchar *pt = mask.ptr<uchar>(i);
		for(int j=0; j<mask.cols; j++){
			pt[j]=255;
		}
	}

	Scalar s = mean(img, mask);
	int mean = (int)s[0];
	cout<<mean<<endl;

	for(int i=0; i<10; i++){
		int prev_mean = mean;

		Mat G1_mask = img.clone();
		Mat G2_mask = img.clone();

		for(int i=0; i<img.rows; i++){
			for(int j=0; j<img.cols; j++){
				if(img.at<uchar>(i, j) > mean) {
					G1_mask.at<uchar>(i,j) = 255;
					G2_mask.at<uchar>(i,j) = 0;
				} else {
					G1_mask.at<uchar>(i,j) = 0;
					G2_mask.at<uchar>(i,j) = 255;
				}
				
			}
		} // end of mask

		s = cv::mean(img, G1_mask);
		int mask1mean = (int)s[0];

		s = cv::mean(img, G2_mask);
		int mask2mean = (int)s[0];

		mean = (mask1mean + mask2mean)/2;
		cout<<mean<<endl;

		if(abs(mean-prev_mean) < 2)
			break;

	}

	return mean;
	
}

void thresholding()
{
	Mat img = imread("lena.JPG", CV_LOAD_IMAGE_GRAYSCALE);
	//binary_thresholding(img, 100, "binary");

	int mean= find_global_threshold(img);
	binary_thresholding(img, mean, "binary threshold");

}


int main(int argc, char** argv)
{
	
	//load_and_show_image();
	//im_show();
	//simpleTransform();
	//image_info();
	//gamma_transform();
	//intensity_slicing();
	//spatial_filtering();
	//sharpen();
	//erosion();
	//edge_detector();
	//dilation();
	//finger_print();
	//histogram_equalization();
	//histogram_matching();
	//shapen();

	fourier_transform_();
	//thresholding();

	waitKey(0);
	getchar();
	return 0;
}