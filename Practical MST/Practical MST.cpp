class Solution {
public:
    void reorderList(ListNode* head) {
        if(!head) return;
        vector<ListNode*> x;
        ListNode* ptr= head;
        while(ptr){
            x.push_back(ptr);
            ptr= ptr->next;
        }
        int i= 0;
        int j= x.size()-1;
        while(i<j){
            x[i]->next= x[j];
            i++;
            if(i==j){
                break;
            }
            x[j]->next= x[i];
            j--;
        }
        x[i]->next= nullptr;
    }
};
