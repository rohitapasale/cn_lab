#include <iostream>
#include <vector>
#include <set>
using namespace std;
 
int main() {
    int totalFrames, windowSize, lostFrame;
    cout << "Enter total number of frames: ";
    cin >> totalFrames;
    cout << "Enter window size (N): ";
    cin >> windowSize;
    cout << "Enter frame number to simulate as lost (0-indexed, -1 for none): ";
    cin >> lostFrame;
 
    vector<bool> acked(totalFrames, false);
    set<int> retransmittedFrames;
    int base = 0;
 
    while (base < totalFrames) {
        int windowEnd = min(base + windowSize, totalFrames);
        cout << "\nSending frames " << base << " to " << windowEnd - 1 << " (window)" << endl;
 
        for (int i = base; i < windowEnd; i++) {
            if (i == lostFrame) {
                cout << "  Frame " << i << " -> LOST in transit" << endl;
            } else {
                acked[i] = true;
                cout << "  Frame " << i << " -> Received & individually ACKed" << endl;
            }
        }
 
        if (lostFrame >= base && lostFrame < windowEnd) {
            cout << "  Timeout for frame " << lostFrame
                 << " only. Selective Repeat: retransmitting frame " << lostFrame << endl;
            acked[lostFrame] = true;
            retransmittedFrames.insert(lostFrame);
            lostFrame = -1;
        }
 
        // Slide window past all consecutively acknowledged frames
        while (base < totalFrames && acked[base]) base++;
        if (base < windowEnd) base = windowEnd;   // ensure progress
    }
 
    cout << "\nTotal frames individually retransmitted (Selective Repeat): "
         << retransmittedFrames.size() << endl;
    return 0;
}
