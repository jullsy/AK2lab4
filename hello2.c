// SPDX-License-Identifier: GPL-2-Clause
#include <hello1.h>

MODULE_AUTHOR("Yuliia Tyshkevych\n");
MODULE_DESCRIPTION("AK-2 Labwork 4\n");
MODULE_LICENSE("Dual BSD/GPL");

// Параметр модуля для встановлення значення chrono_counter
static uint chrono_counter = 1;

module_param(chrono_counter, uint, 0);
MODULE_PARM_DESC(chrono_counter, "Simple chrono counter for my lab\n");

// Функція ініціалізації модуля hello2
static int __init init_hello(void)
{
	pr_info("Success! hello2 has been initiated\n");
	print_hello(chrono_counter); // Виклик функції print_hello з переданим параметром chrono_counter
	return 0; // Успішна ініціалізація
}

// Функція видалення модуля hello2
static void __exit exit_hello(void)
{
	pr_info("Success! hello2 has been unloaded\n"); // Повідомлення про видалення модуля
}

module_init(init_hello); // Вказівка для ядра Linux про виклик функції під час ініціалізації
module_exit(exit_hello); // Вказівка для ядра Linux про виклик функції під час видалення модуля


