#include "loadLibrary.hpp"
#include <thread>
#include <mutex>
#include <vector>

namespace {
    class synchronizer
    {
        std::mutex & mutex_;
    public:
        synchronizer(std::mutex & mutex)
        : mutex_(mutex)
        {
            mutex_.lock();
        }
        ~synchronizer()
        {
            mutex_.unlock();
        }
    };
    
    struct point
    {
        double x;
        double y;
    };
}

class flappy_bird
{
    static const int ana_size = 9; /// 穴のサイズ
    static const int G = 30; /// 重力加速度
    static const int V = -15; /// ジャンプ力
    /** トリのドット絵 */
    const int BIRD[4][6] = {
        {0xFFFF00, 0x000000, 0xFFFFFF, 0xFFFFFF, 0xFFFFFF, 0x000000},
        {0xFFFF00, 0xFFFF00, 0xFFFFFF, 0xFFFFFF, 0x000000, 0xFFFFFF},
        {0xFFFF00, 0xFFFF00, 0xFFFF00, 0xFFFF00, 0xFFFFFF, 0xFFFFFF},
        {0x000000, 0xFFFF00, 0xFFFF00, 0xFF0000, 0xFF0000, 0xFF0000}
    };

    std::vector<int> kabe_; /// 壁のドット位置（Y座標）
    int kabez_; /// 壁のZ座標
    bool game_over_; /// ゲームオーバーフラグ
    std::mutex mutex_; /// ミューテックス
    point birdp_; /// トリの位置
    double v_; /// 落下速度
    std::chrono::system_clock::time_point time_; /// 前回位置計算した時刻
public:
    /*!
     コンストラクタ内部でFlappy Birdの全ての処理を行う（ゲーム開始からゲームオーバーまで）
     */
    flappy_bird()
    : kabez_(-1)
    , game_over_(false)
    , v_(V)
    , birdp_({LED_WIDTH / 2 - 3, LED_HEIGHT / 2 - 2})
    {
        ShowMotioningText1("321");
        std::thread th1([this](){
            kabe_thread();
        });
        std::thread th2([this](){
            key_thread();
        });
        main_thread();
        show_message("GAMEOVER");
        th1.join();
        th2.join();
    }
private:
    /*!
     壁の移動をするスレッド
     ゲームオーバーになるまで処理を継続
     */
    void kabe_thread()
    {
        while(!game_over_){
            {
                synchronizer sync(mutex_);
                if(kabez_ <= 0){
                    kabez_ = LED_DEPTH - 1;
                    new_kabe();
                }else{
                    --kabez_;
                }
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(300)); // TODO define kabe insterval
        }
    }
    /*!
     キー入力を受けるスレッド
     ゲームオーバーになるまで処理を継続
     */
    void key_thread(){
        while(!game_over_){
            std::string input;
            std::cin >> input; // cinだとEnter入力が必須になるので本意ではないが、リアルセンスと組み合わせるならば不要なので気にしない
            {
                synchronizer sync(mutex_);
                v_ = V;
            }
        }
    }
    /*!
     トリの位置を更新する
     壁、天井、床いずれかにぶつかったらゲームオーバーにして、処理を終了する
     */
    void main_thread()
    {
        time_ = std::chrono::system_clock::now();
        while(!game_over_){
            auto now = std::chrono::system_clock::now();
            double t = static_cast<double>(std::chrono::duration_cast<std::chrono::milliseconds>(now - time_).count()) / 1000;
            time_ = now;
            {
                synchronizer sync(mutex_);
                v_ += G * t;
                birdp_.y += v_ * t;
                Clear();
                if(crash()){
                    game_over_ = true;
                    ShowFirework(birdp_.x, birdp_.y, 0);
                }else{
                    set_kabe();
                    set_bird();
                    Show();
                }
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
        }
    }
    /*!
     新しい壁を作る
     */
    void new_kabe()
    {
        int top = rand() % (LED_HEIGHT - ana_size);
        int bottom = top + ana_size;
        kabe_.clear();
        for(int y = 0; y < LED_HEIGHT; ++y){
            if(y < top || bottom <= y){
                kabe_.push_back(y);
            }
        }
    }
    /*!
     壁、天井、床接触判定
     */
    bool crash()
    {
        if(birdp_.y < 0 || LED_HEIGHT - 4 < birdp_.y){
            return true;
        }
        if(0 != kabez_){
            return false;
        }
        for(auto y : kabe_){
            if(birdp_.y <= y && y < birdp_.y + 4){
                return true;
            }
        }
        return false;
    }
    /*!
     トリのドット絵をLEDにセットする
     */
    void set_bird()
    {
        for(int x = 0; x < 6; ++x){
            for(int y = 0; y < 4; ++y){
                SetLed(x + birdp_.x, y + birdp_.y, 0, BIRD[y][x]);
            }
        }
    }
    /*!
     壁のドット絵をLEDにセットする
     */
    void set_kabe()
    {
        for(int x = 0; x < LED_WIDTH; ++x){
            for(auto y : kabe_){
                SetLed(x, y, kabez_, 0x00FF00);
            }
        }
    }
    /*!
     文字を右から左に流す
     */
    void show_message(std::string msg){
        int len = static_cast<int>(msg.size() * LED_WIDTH);
        for(int x = LED_WIDTH; -len < x; --x){
            Clear();
            for(int i = 0; i < msg.size(); ++i){
                int xx = x + i * LED_WIDTH;
                if(-LED_WIDTH <= xx && xx <= LED_WIDTH){
                    SetChar(xx, 0, 0, msg[i], 0x00FF00);
                }
            }
            Show();
        }
    }
};

int main(int argc, const char* argv[])
{
    static_cast<void>(argc); // unused
    static_cast<void>(argv); // unused
#if defined(WIN32) || defined(WIN64)
    loadLibrary("ledLib.dll");
#else if defined(__APPLE__)
    loadLibrary("./libledLib.dylib");
#endif
    if (1 < argc){
        SetUrl(argv[1]);
    }
    for(;;){
        flappy_bird fb;
    }
    return 0;
}
