#ifndef _COLOR_CONFIDENCE_H_
#define _COLOR_CONFIDENCE_H_

/*
*-------------EXAMPLE-------------
* 
* <...>
* 
* maxwolf8852::setBounds(0.5f, 1.0f);
* auto color = maxwolf8852::confidence2Color(0.678f);
* cv::Mat image(480, 640, CV_8UC3, color);
* cv::imshow("conf_color", image);
* cv::waitKey();
* 
* <...>
* 
*---------------------------------
*/


#include <iostream>
#include <utility>

#include <opencv2/opencv.hpp>

namespace maxwolf8852
{
	static void setBounds(float low, float high);

    static void setSaturation(uint8_t _s);

    static void setValue(uint8_t _v);

    static void setHueSpace(uint8_t low, uint8_t high);

	static cv::Scalar confidence2Color(float confidence);
}


/*************************back-end********************************/

namespace __internal_maxwolf8852 {
	static struct __params {
		std::pair<float, float> _bounds;
		bool _boundsSet = 0;
		std::pair<uint8_t, uint8_t> _H = std::make_pair<uint8_t, uint8_t>(0, 80);
		uint8_t _S = 255;
		uint8_t _V = 255;
	} _currentParams;
}


void maxwolf8852::setBounds(float low, float high)
{
	if (low >= high)
		throw std::logic_error("low must be less than high!");

    __internal_maxwolf8852::_currentParams._bounds = std::make_pair(low, high);
    __internal_maxwolf8852::_currentParams._boundsSet = true;
}

void maxwolf8852::setSaturation(uint8_t _s){
        __internal_maxwolf8852::_currentParams._S = _s;
}

void maxwolf8852::setValue(uint8_t _v){
        __internal_maxwolf8852::_currentParams._V = _v;
}

void maxwolf8852::setHueSpace(uint8_t low, uint8_t high){
    if (low >= high || high > 180)
        throw std::out_of_range("low < high && high <= 180");
    __internal_maxwolf8852::_currentParams._H = std::make_pair(low, high);
}

cv::Scalar maxwolf8852::confidence2Color(float confidence)
{
    if (!__internal_maxwolf8852::_currentParams._boundsSet)
		throw std::logic_error("bounds must be set first!");

	if (confidence > __internal_maxwolf8852::_currentParams._bounds.second
		|| confidence < __internal_maxwolf8852::_currentParams._bounds.first)
		throw std::out_of_range("confidence out of bounds!");

	/*
	*            (current_conf - low_conf) * (high_H - low_H)
	* current =   ------------------------------------------    +   low_H
	*						(high_conf - low_conf)
	*/

    uint8_t _curH = (confidence - __internal_maxwolf8852::_currentParams._bounds.first) *
		(__internal_maxwolf8852::_currentParams._H.second - __internal_maxwolf8852::_currentParams._H.first) /
		(__internal_maxwolf8852::_currentParams._bounds.second - __internal_maxwolf8852::_currentParams._bounds.first) + __internal_maxwolf8852::_currentParams._H.first;


    cv::Scalar_<uint8_t> output = cv::Scalar_<uint8_t>(_curH, __internal_maxwolf8852::_currentParams._S
                                    , __internal_maxwolf8852::_currentParams._V);



    cv::Mat colorMat(1,1, CV_8UC3, output);

    cv::cvtColor(colorMat, colorMat, cv::COLOR_HSV2BGR);

    auto data = (uint8_t*)colorMat.data;

    return cv::Scalar(data[0], data[1], data[2]);
}

/******************************************************************/

#endif // _COLOR_CONFIDENCE_H_
