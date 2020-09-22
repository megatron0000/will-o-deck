#include <assert.h>
#include <iostream>

double card_attributes[][7] = {
    {1, 1, 1, 1, 1, 1, 1},
    {2, 2, 2, 2, 2, 2, 2},
    {0, 0, 1, 0, 1, 0, 1},
    {0, 1, 2, 0, 2, 2, 2},
    {2, 2, 0, 0, 0, 2, 0},
    {2, 2, 1, 1, 1, 0, 0},
    {3, 0, 2, 1, 0, 3, 1},
    {3, 1, 0, 1, 3, 3, 2},
    {2, 1, 1, 1, 1, 2, 0},
    {2, 1, 0, 1, 0, 0, 0},
    {2, 2, 1, 1, 1, 1, 1},
    {3, 2, 1, 2, 1, 3, 2},
    {3, 1, 2, 2, 3, 3, 1},
    {1, 2, 2, 2, 2, 1, 2},
    {3, 2, 3, 2, 1, 1, 0},
    {1, 1, 2, 1, 1, 1, 1},
    {1, 0, 2, 0, 1, 1, 2},
    {1, 1, 0, 3, 2, 2, 1},
    {3, 3, 1, 2, 1, 0, 0},
    {0, 2, 0, 1, 1, 2, 2},
    {0, 1, 2, 2, 3, 0, 3},
    {3, 1, 3, 2, 3, 3, 1},
    {1, 1, 1, 1, 1, 1, 1},
    {0, 1, 2, 1, 1, 1, 1},
    {1, 1, 3, 2, 1, 1, 2},
    {1, 3, 1, 3, 0, 0, 3},
    {0, 3, 0, 1, 1, 0, 3},
    {0, 0, 0, 1, 0, 1, 3},
    {1, 0, 1, 3, 0, 0, 3},
    {0, 0, 0, 2, 0, 2, 3},
    {0, 0, 0, 2, 1, 0, 3}};

double card_costs[] = {
    350,
    700,
    150,
    450,
    300,
    350,
    500,
    650,
    400,
    200,
    450,
    700,
    750,
    600,
    600,
    400,
    350,
    500,
    500,
    400,
    550,
    800,
    350,
    350,
    550,
    550,
    400,
    250,
    400,
    350,
    300};

const int quant_cards = sizeof(card_costs) >> 3;

double group_last_cards[] = {7, 13, 18, 30};

int group_lengths[] = {8, 6, 5, 12};

double expected[6] = {19, 7, 11, 15, 17, 16};

double expected_total = 19 + 7 + 11 + 15 + 17 + 16;

int best[quant_cards];

double best_profit = -1;

unsigned long int count = 0;

void process(int group_count, int *group_lengths, int total_length, int *data)
{
  // int start = 0;
  // for (int group = 0; group < group_count; group++) {
  //   int group_length = group_lengths[group];
  //   for (int i = start; i < start + group_length; i++) {
  //     std::cout << data[i] << " ";
  //   }
  //   start += group_length;
  //   if (group != group_count - 1) {
  //     std::cout << ". ";
  //   }
  // }
  // std::cout << std::endl;

  // int card_total = 0;
  double spent_total = 0;
  double total_attr[7] = {0, 0, 0, 0, 0, 0, 0};
  int repeated = data[3] + data[7] + data[33] + data[37] + data[38] + data[53] +
                 data[54] + data[69];
  if (repeated > 4)
    return;
  // bool has_group[] = {false, false, false, false};
  for (int i = 0; i < total_length; i++)
  {
    if (data[i] == 0)
      continue; // card is not present
    // card_total++;
    // assert(card_total <= 8);
    // has_group[i <= group_last_cards[0]
    //               ? 0
    //               : i <= group_last_cards[1]
    //                     ? 1
    //                     : i <= group_last_cards[2] ? 2 : 3] = true;
    spent_total += card_costs[i];
    for (int j = 0; j < 7; j++)
    {
      total_attr[j] += card_attributes[i][j];
    }
  }
  // for (int k = 0; k < 4; k++) {
  //   assert(has_group[k] == true);
  // }

  double inner_product = 0;
  for (int j = 0; j < 6;
       j++)
  { // until 6 because "attribute cost" does not count
    inner_product +=
        (total_attr[j] < expected[j] ? total_attr[j] : expected[j]);
  }
  inner_product /= expected_total;

  if (total_attr[6] > 20)
    return;

  double pitch_score = 0.8;
  double cost_ease = (1 - pitch_score * total_attr[6] * 0.02);
  if (cost_ease <= 0)
    return;

  double remaining = 10133 - spent_total;
  if (remaining <= 0)
    return;

  double profit =
      (1 / (100 * cost_ease)) * remaining * 250 * inner_product * pitch_score;

  if (profit > best_profit)
  {
    best_profit = profit;
    for (int i = 0; i < quant_cards; i++)
    {
      best[i] = data[i];
    }
  }

  count++;
  if (count % 1000000 == 0)
  {
    std::cout << "count: " << count << ". profit: " << best_profit << std::endl;
  }
}

void cycle_multi_one(int group, int group_count, int *group_lengths,
                     int total_length, int *out, int start, int end, int n,
                     bool group_has_one)
{
  int length = end - start + 1;
  int my_group_start = 0;
  for (int i = 0; i < group; i++)
  {
    my_group_start += group_lengths[i];
  }
  if (n == 0)
  {
    for (int i = start; i <= end; i++)
    {
      out[i] = 0;
    }
    process(group_count, group_lengths, total_length, out);
    return;
  }
  if (n > length)
  { // too many to choose. Impossible
    return;
  }
  if (n == group_count - group - 1 &&
      group_has_one)
  { // n is small. Can fit at most one per group, and my
    // group already has one. Forward to the next group
    int next_group_start = 0;
    next_group_start = my_group_start + group_lengths[group];
    for (int i = start; i < next_group_start; i++)
    {
      out[i] = 0;
    }
    cycle_multi_one(group + 1, group_count, group_lengths, total_length, out,
                    next_group_start, end, n, false);
    return;
  }
  if (length == n)
  { // I can only choose all
    int next_group_start = my_group_start + group_lengths[group];
    for (int i = start; i < next_group_start; i++)
    {
      out[i] = 1;
    }
    cycle_multi_one(group + 1, group_count, group_lengths, total_length, out,
                    next_group_start, end, n - (next_group_start - start),
                    false);
    return;
  }
  if (start == my_group_start + group_lengths[group] - 1 &&
      group != group_count - 1)
  { // I am in the last position of the
    // group (except in the last group)
    if (!group_has_one)
    { // No one chose an element of the group and I have to
      // do it because I am in the last position of the
      // group
      out[start] = 1;
      cycle_multi_one(group + 1, group_count, group_lengths, total_length, out,
                      start + 1, end, n - 1, false);
      return;
    }
    out[start] = 0;
    cycle_multi_one(group + 1, group_count, group_lengths, total_length, out,
                    start + 1, end, n, false);
    out[start] = 1;
    cycle_multi_one(group + 1, group_count, group_lengths, total_length, out,
                    start + 1, end, n - 1, false);
    return;
  }
  if (group == group_count - 1 && !group_has_one &&
      start ==
          my_group_start + group_lengths[group] -
              1)
  { // No one chose an element of the group and I have to
    // do it because I am in the last position of the last group
    out[start] = 1;
    process(group_count, group_lengths, total_length, out);
    return;
  }
  out[start] = 0;
  cycle_multi_one(group, group_count, group_lengths, total_length, out,
                  start + 1, end, n, group_has_one);
  out[start] = 1;
  cycle_multi_one(group, group_count, group_lengths, total_length, out,
                  start + 1, end, n - 1, true);
}

void cycle_multi(int group_count, int *group_lengths, int total_length,
                 int *out, int n)
{
  cycle_multi_one(0, group_count, group_lengths, total_length, out, 0,
                  total_length - 1, n, false);
}

int main()
{
  int *arr = new int[quant_cards];
  cycle_multi(4, group_lengths, quant_cards, arr, 8);

  int start = 0;
  for (int group = 0; group < 4; group++)
  {
    int group_length = group_lengths[group];
    for (int i = start; i < start + group_length; i++)
    {
      std::cout << best[i] << " ";
    }
    start += group_length;
    if (group != 4 - 1)
    {
      std::cout << ". ";
    }
  }
  std::cout << std::endl;
}
