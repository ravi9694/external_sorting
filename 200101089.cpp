#include <bits/stdc++.h>
using namespace std;

int record_per_page;
int total_number_of_record;
int number_of_buffer_pages;
int total_pages;

struct page
{
    int *data;     // to store data
    int count = 0; // To count stored value in that page
};

void insertion(page pg[], int value)
{
    for (int i = 0; i < total_pages; i++)
    {
        if (pg[i].count < record_per_page)
        {
            pg[i].data[pg[i].count] = value;
            pg[i].count++;
            return;
        }
    }
}

void display(page pg[])
{
    //cout << "START" << endl;
    for (int i = 0; i < total_pages; i++)
    {
        for (int j = 0; j < pg[i].count; j++)
        {
            cout << pg[i].data[j] <<endl;
        }
    }
    //cout << endl;
    return;
}

void external_sorting(page A[], page B[])
{
    int round = 1;
    page temp[total_pages];
    for (int i = 0; i < total_pages; i++)
    {
        temp[i].data = new int[record_per_page];
    }

    int total_group;
    if (total_pages % number_of_buffer_pages == 0)
    {
        total_group = total_pages / number_of_buffer_pages;
    }
    else
    {
        total_group = total_pages / number_of_buffer_pages + 1;
    }
    // step 0 :
    vector<int> tp;
    for (int i = 0; i < total_group; i++)
    {
        for (int j = 0; j < number_of_buffer_pages; j++)
        {
            if (i * number_of_buffer_pages + j < total_pages)
            {
                for (int k = 0; k < A[i * number_of_buffer_pages + j].count; k++)
                {
                    tp.push_back(A[i * number_of_buffer_pages + j].data[k]);
                }
            }
        }
        sort(tp.begin(), tp.end());
        for (auto x : tp)
        {
            insertion(B, x);
        }
        tp.clear();
    }
    // for steps > 0 :
    int min, min_index; // min is the minimum element and min index is the index
    // of the group which has min value
    int flag;
    int power;
    int real_buffer_page = number_of_buffer_pages - 1; // number of buffer pages available for input
    int temp_array[real_buffer_page][3];
    // 3->>>
    // index 0 is for element value in the page we are reading
    // index 1 is for index of that element in that page
    // index 2 is for index of page in that group
    while ((int)pow(real_buffer_page, (round - 1)) * number_of_buffer_pages < total_pages)
    {
        //display(B);
        power = pow(real_buffer_page, round - 1) * number_of_buffer_pages; // maximum group size of current round in terms of pages
        if (total_pages % (power * real_buffer_page) == 0)
        {
            total_group = total_pages / (power * real_buffer_page);
        }
        else
        {
            total_group = total_pages / (power * real_buffer_page) + 1;
        }
        for (int i = 0; i < total_group; i++)
        {
            for (int j = 0; j < real_buffer_page; j++)
            {
                if (i * power * real_buffer_page + j * power < total_pages)
                {
                    temp_array[j][0] = B[i * power * real_buffer_page + j * power].data[0];
                }
                else
                {
                    temp_array[j][0] = -1;
                }
                temp_array[j][1] = 1;
                temp_array[j][2] = 1;
            }
            min_index = 0;
            for (int j = 0; j < real_buffer_page * (power)*record_per_page; j++)
            {
                min = INT_MAX;
                flag = 0;
                for (int k = 0; k < real_buffer_page; k++)
                {
                    if (temp_array[k][0] != -1)
                    {
                        flag = 1;
                        if (min > temp_array[k][0])
                        {
                            min = temp_array[k][0];
                            min_index = k;
                        }
                    }
                }
                if (flag == 1)
                {
                    insertion(temp, min);
                }
                if (temp_array[min_index][1] < B[i * power * real_buffer_page + min_index * power + temp_array[min_index][2] - 1].count)
                {
                    temp_array[min_index][0] = B[i * power * real_buffer_page + min_index * power + temp_array[min_index][2] - 1].data[temp_array[min_index][1]];
                    temp_array[min_index][1]++;
                }
                else if (temp_array[min_index][2] < power)
                {
                    if (i * power * real_buffer_page + min_index * power + temp_array[min_index][2] < total_pages)
                    {
                        temp_array[min_index][0] = B[i * power * real_buffer_page + min_index * power + temp_array[min_index][2]].data[0];
                        temp_array[min_index][1] = 1;
                        temp_array[min_index][2]++;
                    }
                    else
                    {
                        temp_array[min_index][0] = -1;
                    }
                }
                else
                {
                    temp_array[min_index][0] = -1;
                }
            }
        }
        for (int i = 0; i < total_pages; i++)
        {
            for (int j = 0; j < temp[i].count; j++)
            {
                B[i].data[j] = temp[i].data[j];
            }
            B[i].count = temp[i].count;
            temp[i].count = 0;
        }
        round++;
    }
    return;
}

int main()
{
    cin >> number_of_buffer_pages;
    cin >> record_per_page;
    cin >> total_number_of_record;
    if (total_number_of_record % record_per_page == 0)
    {
        total_pages = int(total_number_of_record / record_per_page);
    }
    else
    {
        total_pages = int(total_number_of_record / record_per_page) + 1;
    }
    page pages[total_pages];
    page sorted_pages[total_pages];
    for (int i = 0; i < total_pages; i++)
    {
        pages[i].data = new int[record_per_page];
        sorted_pages[i].data = new int[record_per_page];
    }

    // Insert records in pages
    int temp;
    for (int i = 0; i < total_number_of_record; i++)
    {
        cin >> temp;
        insertion(pages, temp);
    }

    // Sort record
    external_sorting(pages, sorted_pages);

    // Print the result
    display(sorted_pages);
}
