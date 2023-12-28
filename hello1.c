// SPDX-License-Identifier: GPL-2-Clause
#include <hello1.h>

MODULE_AUTHOR("Yuliia Tyshkevych\n");
MODULE_DESCRIPTION("AK-2 Labwork 4\n");
MODULE_LICENSE("Dual BSD/GPL");

// Структура для вимірювання часу перед та після виклику функції
struct timeit_list {
	struct list_head node; // Елемент списку для зберігання даних
	ktime_t before;        // Час перед викликом
	ktime_t after;         // Час після виклику
};

static struct list_head head_node = LIST_HEAD_INIT(head_node); // Ініціалізація головного списку

// Функція для виведення привітання та вимірювання часу
int print_hello(uint chrono_counter)
{
	struct timeit_list *list, *tmp; // Вказівники на структури для вимірювання часу
	uint i;

	if (chrono_counter < 0) {
		pr_err("Fail! The parameter's less than 0\n");
	} else if (chrono_counter == 0) {
		pr_warn("Warning! The parameter's 0\n\n");
	} else if (chrono_counter >= 5 && chrono_counter <= 10) {
		pr_warn("Warning! The parameter's between 5 and 10\n");
	} else if (chrono_counter > 10) {
		pr_err("Fail! The parameter's > 10\n");
		return -EINVAL; // Повернення помилки
	}

	for (i = 0; i < chrono_counter; i++) {
		list = kmalloc(sizeof(struct timeit_list), GFP_KERNEL); // Виділення пам'яті під структуру
		if (ZERO_OR_NULL_PTR(list))
			goto tidy_away; // Перехід до очищення пам'яті в разі невдачі
		list->before = ktime_get(); // Запис часу перед викликом
		pr_info("Hello, world!\n"); // Виведення привітання
		list->after = ktime_get();  // Запис часу після виклику
		list_add_tail(&list->node, &head_node); // Додавання структури до списку
	}
	return 0;

tidy_away:
	list_for_each_entry_safe(list, tmp, &head_node, node) {
		list_del(&list->node); // Видалення елемента зі списку
		kfree(list); // Звільнення виділеної пам'яті
	}
	pr_err("Fail! No memory\n");
	return -1; // Повернення помилки
}
EXPORT_SYMBOL(print_hello); // Вказівка для ядра Linux про доступ до функції ззовні

static int __init init_hello(void)
{
	pr_info("Success! hello1 has been initiated\n");
	return 0; // Успішна ініціалізація
}

static void __exit exit_hello(void)
{
	struct timeit_list *list, *tmp;

	list_for_each_entry_safe(list, tmp, &head_node, node) {
		pr_info("Time: %lld", list->after - list->before); // Виведення виміряного часу
		list_del(&list->node); // Видалення елемента зі списку
		kfree(list); // Звільнення виділеної пам'яті
	}
	BUG_ON(!list_empty(&head_node)); // Перевірка на відсутність елементів у списку
	pr_info("Success! hello1 has been unloaded\n"); // Повідомлення про видалення модуля
}

module_init(init_hello); // Вказівка для ядра Linux про виклик функції під час ініціалізації
module_exit(exit_hello); // Вказівка для ядра Linux про виклик функції під час видалення модуля

