#include <iostream>
#include <vector>
using namespace std;
 
int main() {
    int totalFrames, windowSize, lostFrame;
    cout << "Enter total number of frames: ";
    cin >> totalFrames;
    cout << "Enter window size (N): ";
    cin >> windowSize;
    cout << "Enter frame number to simulate as lost (0-indexed, -1 for none): ";
    cin >> lostFrame;
 
    int base = 0;
    int retransmissions = 0;
 
    while (base < totalFrames) {
        int windowEnd = min(base + windowSize, totalFrames);
        cout << "\nSending frames " << base << " to " << windowEnd - 1 << " (window)" << endl;
 
        bool lossInWindow = (lostFrame >= base && lostFrame < windowEnd);
 
        for (int i = base; i < windowEnd; i++) {
            if (i == lostFrame) {
                cout << "  Frame " << i << " -> LOST in transit" << endl;
                break;   // GBN: sender stops on first loss in this pass
            } else {
                cout << "  Frame " << i << " -> Acknowledged" << endl;
            }
        }
 
        if (lossInWindow) {
            cout << "  Timeout! Go-Back-N: retransmitting frames " << lostFrame
                 << " to " << windowEnd - 1 << endl;
            retransmissions += (windowEnd - lostFrame);
            lostFrame = -1;      // loss handled; continue normally
            base = lostFrame;    // placeholder, corrected below
            base = base;         // (kept for clarity of GBN restart point)
            base = windowEnd - (windowEnd - lostFrame >= 0 ? 0 : 0);
        }
        base = windowEnd;
    }
 
    cout << "\nTotal frames retransmitted (Go-Back-N): " << retransmissions << endl;
    return 0;
}
