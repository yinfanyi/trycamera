#include "camera.hpp"

#include "libobsensor/hpp/Pipeline.hpp"
#include "libobsensor/hpp/Error.hpp"
#include <iostream>
#include <opencv2/opencv.hpp>

int main(int argc, char **argv) try {

    auto camera = Camera::getInstance();
    camera->start();
    std::shared_ptr<ob::DepthFrame> depthFrame;
    std::shared_ptr<ob::ColorFrame> colorFrame;

    cv::namedWindow("Image Window", cv::WINDOW_AUTOSIZE);

    while(true) {

        auto frames_ = camera->receiveFrames();

        if(frames_==std::nullopt){continue;}

        depthFrame = frames_->depthFrame;
        colorFrame = frames_->colorFrame;

        if(depthFrame->index() % 30 == 0 && depthFrame->format() == OB_FORMAT_Y16) {
            auto distance = camera->getDistance(320, 200);
            std::cout << distance.value() << std::endl;
        }

        // Render frame in the window
        // camera->app->addToRender({colorFrame, depthFrame});
        
        cv::imshow("Image Window", camera->colorImage);
        cv::waitKey(1);
    }
    // Stop the pipeline
    camera->stop();
    return 0;
}
catch(ob::Error &e) {
    std::cerr << "function:" << e.getName() << "\nargs:" << e.getArgs() << "\nmessage:" << e.getMessage() << "\ntype:" << e.getExceptionType() << std::endl;
    exit(EXIT_FAILURE);
}
