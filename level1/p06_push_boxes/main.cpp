#include <iostream>
#include <windows.h>
#include <vector>
using namespace std;

// ==========================================
// 1. 基础音符频率定义 (12平均律 G调)
// ==========================================
// 基础音 (低音)
#define L5 196
#define L6 220
#define L7 247
// 中音
#define C 262  // 1 (Do)
#define D 294  // 2 (Re)
#define E 330  // 3 (Mi)
#define F 349  // 4 (Fa)
#define G 392  // 5 (Sol)
#define A 440  // 6 (La)
#define B 494  // 7 (Si)
// 高音
#define HC 523 // i (高音Do)
#define HD  588 // i2 (高音Re)
#define HE  660 // i3 (高音Mi)
#define HF  698 // i4 (高音Fa)
#define HG  784 // i5 (高音Sol)
#define HA  880 // i6 (高音La)
#define HB  988 // i7 (高音Si)
// 倍高音
#define VHC 1047 // 1 (倍高音Do)

// 半音/变音
#define D_SHARP 311 // #2 (Re#)
#define F_SHARP 370 // #4 (Fa#)
#define G_SHARP 415 // #5 (Sol#)

// ==========================================
// 2. 核心播放逻辑结构体
// ==========================================

// 用于存储单个音符的数据
struct Note {
    int pitch;      // 音高 (Hz)
    int duration;   // 持续时间 (ms)
};

// 用于控制节拍的结构体
struct Rhythm {
    Note note;
};

// 演奏控制类
class MusicPlayer {
public:
    // 速度控制：基础四分音符时长 (ms)，越大越慢
    int beat_duration;

    MusicPlayer(int bpm_ms = 400) : beat_duration(bpm_ms) {}

    // 将简谱符号转换为音符数组
    // 0: 无音(休止), 1-7: 中音, i: 高音, .: 附点, _: 下划线(八分音符), #: 升号
    // 格式: "1" (四分音符1), "05" (八分休止+八分5), "1." (附点四分音符1)
    vector<Rhythm> parseMelody(const vector<string>& symbols) {
        vector<Rhythm> rhythm_track;

        for (const string& s : symbols) {
            Note n;
            int multiplier = 1;

            // 1. 判断时值 (下划线或点)
            if (s.find('_') != string::npos) {
                multiplier = 0.5; // 八分音符
            } else if (s.find('.') != string::npos) {
                multiplier = 1.5; // 附点音符
            } else {
                multiplier = 1.0; // 默认四分音符
            }

            // 2. 解析纯音符字符
            string pitch_str = s;
            size_t pos = pitch_str.find_first_of("._#");
            if (pos != string::npos) pitch_str.erase(pos);

            n.duration = beat_duration * multiplier;

            // 3. 匹配音高
            if (pitch_str == "0" || pitch_str.empty()) {
                n.pitch = 0; // 休止符
            } else if (pitch_str == "L5") n.pitch = L5;
            else if (pitch_str == "L6") n.pitch = L6;
            else if (pitch_str == "L7") n.pitch = L7;
            else if (pitch_str == "1") n.pitch = C;
            else if (pitch_str == "2") n.pitch = D;
            else if (pitch_str == "3") n.pitch = E;
            else if (pitch_str == "4") n.pitch = F;
            else if (pitch_str == "5") n.pitch = G;
            else if (pitch_str == "6") n.pitch = A;
            else if (pitch_str == "7") n.pitch = B;
            else if (pitch_str == "i1") n.pitch = HC;
            else if (pitch_str == "i2") n.pitch = HD;
            else if (pitch_str == "i3") n.pitch = HE;
            else if (pitch_str == "i4") n.pitch = HF;
            else if (pitch_str == "i5") n.pitch = HG;
            else if (pitch_str == "i6") n.pitch = HA;
            else if (pitch_str == "i7") n.pitch = HB;
            else if (pitch_str == "v1") n.pitch = VHC;
            // 变音/半音处理
            else if (pitch_str == "#2") n.pitch = D_SHARP;
            else if (pitch_str == "#4") n.pitch = F_SHARP;
            else if (pitch_str == "#5") n.pitch = G_SHARP;
            else n.pitch = 0; // 默认静音

            rhythm_track.push_back({n});
        }
        return rhythm_track;
    }

    // 播放整个音乐
    void play(const vector<Rhythm>& track) {
        for (const auto& r : track) {
            if (r.note.pitch > 0) {
                Beep(r.note.pitch, r.note.duration);
            } else {
                Sleep(r.note.duration);
            }
            // 音符间的微小间隔，让节奏更清晰
            Sleep(15);
        }
    }
};

// ==========================================
// 3. 完整《晴天》旋律数据录入 (对应原谱)
// ==========================================

// 1. 前奏与主歌 Verse
const vector<string> MELODY_VERSE = {
    // 故事的小黄花 从出生那年就飘着
    "_0", "_5", "5", "_i1", "_i1", "1", "2", "3",
    "_0", "_5", "5", "_i1", "1", "2", "3", "_2i5",
    // 童年的荡秋千 随记忆一直晃到现在
    "_0", "_5", "5", "_i1", "1", "2", "3",
    "_0", "_3", "2", "_3", "4", "3", "2", "4", "_3", "2", "1",
    // 吹着前奏望着天空我 想起花瓣试着掉落
    "_0", "1", "1", "3", "4", "3", "2", "1", "2",
    "3", "3", "3", "3", "2", "3", "_2", "1", "7",
    // 为你翘课的那一天 花落的那一天
    "5", "1", "1", "3", "4", "3", "2", "1", "2",
    "3", "3", "3", "3", "2", "3", "_2", "1", "1.", "7",
    // 教室的那一间 我怎么看不见 消失的下雨天 我好想再淋一遍
    "_i1", "1", "1", "_7", "1", "_i1", "_i1", "1", "1",
    "5", "5", "_5", "5", "5", "5", "_5", "5", "5", "5", "5", "4", "4", "3",
    // 没想到失去的勇气我还留着 啊好想再问一遍
    "3", "_0", "_i1", "_i1", "_i1", "1", "_6", "7", "1", "5", "4", "3", "1", "1",
    "1", "_06", "_6", "_i1", "_i1", "1", "3", "1"
};

// 2. 导歌 Pre-Chorus (好想你...)
const vector<string> MELODY_PRE = {
    // 你会等待还是离开
    "_6", "7", "_1", "5", "4", "_3", "1", "2", "2", "_0", "_0",
    // 刮风这天我试过握着你手 但偏偏雨渐渐大到我看你不见
    "_3", "2", "4", "3", "1", "5", "7", "_i1", "7", "5", "1", "1", "_6", "6",
    "_6", "6", "_5", "5", "5", "4", "3", "2", "3", "4", "3", "3", "_0",
    // 还要多久我才能在你身边 等到放晴的那天
    "3", "#4", "#5", "3", "4", "5", "7", "_i2", "7", "_i1", "_i1", "_i1.", "_i1",
    "_i1", "5", "5", "6", "5", "_4", "4", "2", "3"
};

// 3. 副歌 Chorus (从前从前...)
const vector<string> MELODY_CHORUS = {
    // 也许我会比较好一点
    "4", "5", "6", "1", "_6.", "_7", "7",
    // 从前从前有个人爱你很久 但偏偏风渐渐把距离吹得好远
    "_3", "2", "4", "3", "_0", "1", "5", "7", "_i1", "7", "5", "_1", "1", "_6", "6",
    "_0", "6", "_5", "_5", "_0", "5", "4", "3", "2", "3", "4", "3", "3", "_0",
    "3", "#4", "#5", "3", "4", "5", "7", "_i2", "7", "_i1", "_i1", "_i1.", "_i1",
    "_i1", "5", "5", "6", "5", "_4", "4", "_6", "_7",
    // 好不容易又能再多爱一天 但故事的最后你好像还是说了 oh bye-bye
    "1", "2", "_3", "2", "_0", "1", "3", "_1", "1", "_0", "_0", "_0.7"
};

// 4. 间奏与重复
const vector<string> MELODY_INTERLUDE = {
    "2", "_0", "_0", "0", "0", "0", "0" // 简单过门
};

// ==========================================
// 4. 主函数
// ==========================================
int main() {
    // 设置速度：400ms 为一个基础四分音符 (相当于 BPM = 150)
    MusicPlayer player(400);

    cout << "===== C++ 蜂鸣器正在演奏《晴天》 =====" << endl;
    cout << "请确保系统声音已开启..." << endl;
    cout << "按 Ctrl+C 可随时停止播放。" << endl;
    Sleep(1000);

    // 播放主歌
    cout << "正在播放 [主歌] ..." << endl;
    player.play(player.parseMelody(MELODY_VERSE));

    Sleep(800); // 小节停顿

    // 播放导歌
    cout << "正在播放 [导歌] ..." << endl;
    player.play(player.parseMelody(MELODY_PRE));

    Sleep(800); // 小节停顿

    // 播放副歌
    cout << "正在播放 [副歌] ..." << endl;
    player.play(player.parseMelody(MELODY_CHORUS));

    Sleep(800);

    // 播放第二段副歌 (简化版，实际歌曲中重复)
    cout << "正在播放 [副歌-重复] ..." << endl;
    player.play(player.parseMelody(MELODY_CHORUS));

    cout << "===== 演奏完毕！ =====" << endl;

    return 0;
}