#include <opencv2/opencv.hpp>
#include <iostream>
#include <thread>
#include <vector>
#include <chrono>

using namespace cv;
using namespace std;
using namespace std::chrono;

// Functie pentru oglindirea unei bucati din imagine
void mirror_section(Mat& image, int start_row, int end_row) {
    for (int i = start_row; i < end_row; ++i) {
        for (int j = 0; j < image.cols / 2; ++j) {
            Vec3b& pixel1 = image.at<Vec3b>(i, j);
            Vec3b& pixel2 = image.at<Vec3b>(i, image.cols - 1 - j);
            swap(pixel1, pixel2);
        }
    }
}

int main(int argc, char** argv) {
    // Citirea imaginii
    Mat image = imread("poza6.jpg", IMREAD_COLOR);
    if (image.empty()) {
        cout << "Nu s-a putut încărca imaginea. Verifică dacă calea este corectă." << endl;
        return -1;
    }

    int rows = image.rows;
    int cols = image.cols;
    int num_threads = thread::hardware_concurrency(); // Obține numărul optim de threads
    vector<thread> threads;

    // Calculează dimensiunea unei bucăți de imagine
    int chunk_size = rows / num_threads;

    // Măsurarea timpului de început (doar pentru procesare)
    auto start = high_resolution_clock::now();

    // Crează threads pentru a procesa bucăți din imagine
    for (int i = 0; i < num_threads; ++i) {
        int start_row = i * chunk_size;
        int end_row = (i == num_threads - 1) ? rows : (i + 1) * chunk_size;
        threads.push_back(thread(mirror_section, ref(image), start_row, end_row));
    }

    // Așteaptă finalizarea tuturor threads
    for (auto& t : threads) {
        t.join();
    }

    // Măsurarea timpului de final (doar pentru procesare)
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<milliseconds>(stop - start);

    // Salvează imaginea oglindită și afișează timpul de execuție
    imwrite("output_poza6_threads.jpg", image);
    cout << "Imaginea a fost oglindită și salvată cu succes." << endl;
    cout << "Timpul de execuție pentru procesarea imaginii: " << duration.count() << " milisecunde." << endl;

    return 0;
}
