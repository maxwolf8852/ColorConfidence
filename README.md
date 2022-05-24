# ColorConfidence
C++ header-only library which converts confidence to RGB value for Opencv library.

## Usage

### Install

Install opencv and include this header to your project.

### Example

```C++
maxwolf8852::setBounds(0.5f, 1.0f); // set min and max confidences
auto color = maxwolf8852::confidence2Color(0.678f); // generate color in (R,G,B) scalar
cv::Mat image(480, 640, CV_8UC3, color); // Init cv::MAt with created scalar
cv::imshow("conf_color", image); // show image with generated color
cv::waitKey();
```