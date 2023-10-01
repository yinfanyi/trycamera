#pragma once
#include "window.hpp"
#include <memory>
#include <iostream>
#include <optional>

struct colorAndDepthFrame
{
    std::shared_ptr<ob::ColorFrame> colorFrame;
    std::shared_ptr<ob::DepthFrame> depthFrame;
};

class Camera {
private:
    // static std::shared_ptr<Camera> instance;
    std::shared_ptr<ob::Config> config = std::make_shared<ob::Config>();
    ob::Pipeline pipe;
    std::shared_ptr<ob::VideoStreamProfile> depthProfile = nullptr;
    std::shared_ptr<ob::VideoStreamProfile> colorProfile = nullptr;
    std::shared_ptr<ob::DepthFrame> depthFrame;
    std::shared_ptr<ob::ColorFrame> colorFrame;
    
    Camera() {

        try {
            // Get all stream profiles of the color camera, including stream resolution, frame rate, and frame format
            auto colorProfiles = pipe.getStreamProfileList(OB_SENSOR_COLOR);
            if(colorProfiles) {
                colorProfile = std::const_pointer_cast<ob::StreamProfile>(colorProfiles->getProfile(0))->as<ob::VideoStreamProfile>();
            }
            config->enableStream(colorProfile);
        }
        catch(...) {
            std::cerr << "Current device is not support color sensor!" << std::endl;
            exit(EXIT_FAILURE);
        }

        auto depthProfiles = pipe.getStreamProfileList(OB_SENSOR_DEPTH);
        if(depthProfiles) {
            depthProfile = std::const_pointer_cast<ob::StreamProfile>(depthProfiles->getProfile(0))->as<ob::VideoStreamProfile>();
        }
        config->enableStream(depthProfile);

        // Configure the alignment mode as hardware D2C alignment
        config->setAlignMode(ALIGN_D2C_HW_MODE);
    }

public:
    std::shared_ptr<Window> app;
    uint32_t width = 640;
    uint32_t  height = 400;
    float scale = 1;
    cv::Mat colorImage;
    static std::shared_ptr<Camera> getInstance() {
        static std::shared_ptr<Camera> instance(new Camera());
        return instance;
    }

    std::optional<colorAndDepthFrame> receiveFrames() {
        auto frameSet = pipe.waitForFrames(100);
        if(frameSet == nullptr) {
            return std::nullopt;
        }

        colorFrame = frameSet->colorFrame();
        depthFrame = frameSet->depthFrame();
        
        colorAndDepthFrame frames_;
        frames_.colorFrame = colorFrame;
        frames_.depthFrame = depthFrame;

        // std::cout<<"lala" <<colorFrame->format() << std::endl;
        try{
            cv::Mat rawMat(1, colorFrame->dataSize(), CV_8UC1, colorFrame->data());
            colorImage = cv::imdecode(rawMat, 1);
        }
        catch(...) {
            std::cerr << "Can't get colorImage!" << std::endl;
        }

        

        return frames_;
    }

    std::optional<float> getDistance(int u, int v){
        auto _frames = receiveFrames();
        if(_frames==std::nullopt){return std::nullopt;}
        depthFrame = _frames->depthFrame;
        uint16_t *data = (uint16_t *)depthFrame->data();
        
        if (u > 0 && u < width && v > 0 && v < height) {
            return float(data[width * (v-1) + u] * scale);
        }
        else{
            std::cout<< "u or v out of pixels!"<< std::endl;
            return std::nullopt;
        }
    }

    void start(){
        try {
            pipe.start(config);
        }
        catch(ob::Error &e) {
            std::cerr << "function:" << e.getName() << "\nargs:" << e.getArgs() << "\nmessage:" << e.getMessage() << "\ntype:" << e.getExceptionType() << std::endl;
        }
        // app = std::make_shared<Window>("cameraWindow", colorProfile->width(), colorProfile->height(), RENDER_OVERLAY);
    }

    void stop(){
        pipe.stop();
    }
};

// std::shared_ptr<Camera> Camera::instance = nullptr;
