// ============== FULL Phần AI =================

#include "dl_model_base.hpp"
#include <map>
#include <vector>
#include <string>
#include <iostream>
#include <cmath>

#define NUM_SAMPLES 1
#define NUM_FEATURES 7
// Tên biến chính xác sinh bởi linker từ "models/espdl_model.espdl"
extern const uint8_t model_espdl[] asm("_binary_model_espdl_start");

// Hàm sigmoid 
float sigmoid(float x) {
    return 1.0f / (1.0f + expf(-x));
}

// Dữ liệu thô
float input_data[NUM_SAMPLES][NUM_FEATURES] = {
    //{96, 36.85263343f, 97.12412468f, 72, 0, 71.75897167f, 1.603887873f} // High Risk
    {83, 36.044191f,   98.584497f,   84, 0, 79.295332f,   1.672735f} // Low Risk
    // {63, 37.052, 98.50, 68, 1, 90.31, 1.77}
    //{79, 36.885, 95.9871, 22, 0, 79.8699, 1.92233}
};

const float means[NUM_FEATURES] = {79.53374663, 36.74835291, 97.50437243, 53.44627537, 0.49946505, 74.99641903, 1.75003102};
const float stds[NUM_FEATURES] = {11.55286498, 0.43328918, 1.44259433, 20.78674961, 0.49999971, 14.4714659, 0.14455348};

float standard_scaler(float x, float u, float s) {
    float z = (x - u) / s;
    return z;
}

void run_hvsd_model_batch()
{
    // Load model từ flash
    dl::Model *model = new dl::Model((const char *)model_espdl, fbs::MODEL_LOCATION_IN_FLASH_RODATA);

    ESP_ERROR_CHECK(model->test());
    // Lấy input/output tensor
    dl::TensorBase *model_input = model->get_inputs().begin()->second;
    dl::TensorBase *model_output = model->get_outputs().begin()->second;

    // Chuẩn bị dữ liệu input chuẩn hóa
    float input_scaled[NUM_SAMPLES][NUM_FEATURES];
    for (int i = 0; i < NUM_SAMPLES; ++i) {
        printf("Sample %d:\n", i);
        for (int j = 0; j < NUM_FEATURES; ++j) {
            input_scaled[i][j] = (input_data[i][j] - means[j]) / stds[j];
            printf("Feature %d: raw=%.4f, mean=%.4f, std=%.4f, normalized=%.4f\n", j, input_data[i][j], means[j], stds[j], input_scaled[i][j]);
        }
    }

    // Gán dữ liệu cho input tensor
    dl::TensorBase* input_tensor = new dl::TensorBase({NUM_SAMPLES, NUM_FEATURES}, nullptr, 0, dl::DATA_TYPE_FLOAT);
    memcpy(input_tensor->data, input_scaled, sizeof(input_scaled));
    model_input->assign(input_tensor);

    // Chạy mô hình
    model->run();

    // Lấy và đọc output
    printf("Model output shape: [%zu, %zu]\n", model_output->shape[0], model_output->shape[1]);
    float* raw_output_data = (float*)model_output->data;
    for (int i = 0; i < NUM_SAMPLES; ++i) {
        printf("Raw model output[%d] = %f\n", i, raw_output_data[i]);
    }
    dl::TensorBase* output_tensor = new dl::TensorBase({NUM_SAMPLES, 1}, nullptr, 0, dl::DATA_TYPE_FLOAT);

    output_tensor->assign(model_output);

    float *output_ptr = (float *)output_tensor->data;
    for (int i = 0; i < NUM_SAMPLES; ++i) {
        float logit = output_ptr[i];
        float prob = sigmoid(logit);
        int class_label = (prob >= 0.5f) ? 1 : 0;
        const char* risk = (class_label == 1) ? "Low Risk" : "High Risk";

        printf("Sample %d: logit = %.4f, prob = %.4f -> %s\n", i, logit, prob, risk);

    }

    delete input_tensor;
    delete output_tensor;
    delete model;

    printf("Inference done.\n");
}

// ===== Load & Test & Profile Model =====
extern "C" void app_main(void)
{
    run_hvsd_model_batch();
}
