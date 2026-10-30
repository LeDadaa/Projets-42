/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: damalca <damalca@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/24 11:24:05 by damalca           #+#    #+#             */
/*   Updated: 2026/02/22 13:19:54 by damalca          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Cat.hpp"
#include "Dog.hpp"

int main()
{
    const Animal *tab[6] = {new Cat, new Cat, new Cat, new Dog, new Dog, new Dog};

    Animal animal;
    std::cout << std::endl;

    Cat cat;
    Dog dog;
    cat.getBrain()->initIdeas("lit");
    dog.getBrain()->initIdeas("croquettes");
    cat.getBrain()->printIdeas();
    dog.getBrain()->printIdeas();
    Dog original;
    original.getBrain()->initIdeas("Original thought");
    Dog copy = original;
    copy.getBrain()->initIdeas("Modified thought");
    std::cout << "Original: ";
    original.getBrain()->printIdeas();
    std::cout << "Copy: ";
    copy.getBrain()->printIdeas();

    for (int i = 0; i < 6; i++)
        delete tab[i];
    return (0); 
}