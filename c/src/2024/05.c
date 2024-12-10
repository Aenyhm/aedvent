#include "common/result.h"

Arena arena;

typedef struct {
  int page1;
  int page2;
} Rule;

Array_Template(Rule, Array_Rule);
Array_Template(Array_s64, Array_Update);

typedef struct {
  Array_Rule rules;
  Array_Update updates;
} File_Data;

typedef struct {
  int value;
  Array_s64 nexts;
} Intermediate_Rule;
Array_Template(Intermediate_Rule, Array_Intermediate_Rule);

typedef void (*Fix_Function)(Array_Intermediate_Rule, Array_Update *);

static Rule parse_rule(String s) {
  Rule rule = {0};

  String lhs = str_chop_by_delim(&s, "|");
  rule.page1 = str_to_s64(lhs);
  rule.page2 = str_to_s64(s);

  return rule;
}

static Array_s64 parse_update(String s) {
  Array_s64 pages = {0};

  Array_String pages_strings = str_split(s, ",");
  for (size_t i = 0; i < pages_strings.count; ++i) {
    String item = pages_strings.items[i];
    int page = str_to_s64(item);
    array_append(&pages, page);
  }

  return pages;
}

static File_Data parse_file_data(String * s) {
  File_Data file_data = {0};

  String rules_string   = str_chop_by_delim(s, "\n\n");
  String updates_string = *s;

  Array_String rules = str_split(rules_string, "\n");
  for (size_t i = 0; i < rules.count; ++i) {
    String item = rules.items[i];
    Rule rule = parse_rule(item);
    array_append(&file_data.rules, rule);
  }

  Array_String updates = str_split(updates_string, "\n");
  for (size_t i = 0; i < updates.count; ++i) {
    String item = updates.items[i];
    Array_s64 pages = parse_update(item);
    array_append(&file_data.updates, pages);
  }

  return file_data;
}


//  47: 53,13,61,29
//  53: 29,13
//  97: 13,61,47,29,53,75
//  13:
//  61: 13,53,29
//  75: 29,53,47,61,13
//  29: 13
static Array_Intermediate_Rule rules_to_intermediate_rules(Array_Rule rules) {
  Array_Intermediate_Rule irules = {0};

  for (size_t i = 0; i < rules.count; ++i) {
    Rule rule = rules.items[i];

    int irule_page1_index = -1;
    int irule_page2_index = -1;
    for (size_t j = 0; j < irules.count; ++j) {
      Intermediate_Rule irule = irules.items[j];
      if (irule.value == rule.page1) {
        irule_page1_index = j;
      } else if (irule.value == rule.page2) {
        irule_page2_index = j;
      }
    }

    if (irule_page1_index == -1) {
      Intermediate_Rule irule_page1 = {0};
      irule_page1.value = rule.page1;
      array_append(&irules, irule_page1);
      irule_page1_index = irules.count - 1;
    }

    array_append(&irules.items[irule_page1_index].nexts, rule.page2);

    if (irule_page2_index == -1) {
      Intermediate_Rule irule_page2 = {0};
      irule_page2.value = rule.page2;
      array_append(&irules, irule_page2);
    }
  }

  return irules;
}

static Intermediate_Rule find_irule(Array_Intermediate_Rule irules, int page) {
  Intermediate_Rule result = {0};

  for (size_t i = 0; i < irules.count; ++i) {
    Intermediate_Rule irule = irules.items[i];
    if (irule.value == page) {
      result = irule;
      break;
    }
  }

  return result;
}

static bool is_valid_update(Array_Intermediate_Rule irules, Array_s64 pages) {
  bool result = true;

  for (size_t i = 0; i < pages.count; ++i) {
    int update_page = pages.items[i];
    Intermediate_Rule irule = find_irule(irules, update_page);

    if (!irule.value || i == pages.count - 1) continue;

    bool is_valid;
    array_contains(irule.nexts, pages.items[i + 1], is_valid);

    if (!is_valid) {
      result = false;
      break;
    }
  }

  return result;
}

static void remove_wrong_updates(Array_Intermediate_Rule irules, Array_Update * updates) {
  for (size_t i = 0; i < updates->count;) {
    Array_s64 pages = updates->items[i];

    if (is_valid_update(irules, pages)) {
      i++;
    } else {
      array_remove_at_index(updates, i);
    }
  }
}

static int compute_middle_row(Array_Update updates) {
  int result = 0;

  for (size_t i = 0; i < updates.count; ++i) {
    Array_s64 pages = updates.items[i];
    size_t middle_index = pages.count/2;
    result += pages.items[middle_index];
  }

  return result;
}

static int process(File_Data file_data, Fix_Function fix_fn) {
  Array_Intermediate_Rule irules = rules_to_intermediate_rules(file_data.rules);

  fix_fn(irules, &file_data.updates);

  return compute_middle_row(file_data.updates);
}

static s64 part1(String s) { return process(parse_file_data(&s), remove_wrong_updates); }

int main() {
  arena = arena_alloc(KiB(100));

    String example = read_whole_file(&arena, "data/2024/05/example.txt");
    String input   = read_whole_file(&arena, "data/2024/05/input.txt");

    run(part1, example, 143);
    run(part1, input, 5108);

    // TODO: part 2

  arena_release(&arena);

  return 0;
}
