/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: damalca <damalca@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/24 17:56:55 by damalca           #+#    #+#             */
/*   Updated: 2026/02/22 14:02:06 by damalca          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Amateria.hpp"
#include "Character.hpp"
#include "Cure.hpp"
#include "Ice.hpp"
#include "ICharacter.hpp"
#include "IMateriaSource.hpp"
#include "MateriaSource.hpp"

int main()
{
	//tests du main
	IMateriaSource* src = new MateriaSource();
	src->learnMateria(new Ice());
	src->learnMateria(new Cure());
	ICharacter* me = new Character("me");
	AMateria* tmp;
	tmp = src->createMateria("ice");
	me->equip(tmp);
	tmp = src->createMateria("cure");
	me->equip(tmp);
	ICharacter* bob = new Character("bob");
	me->use(0, *bob);
	me->use(1, *bob);
	delete bob;
	delete me;
	delete src;

	//tests en plus
	Character* hero = new Character("Hero");
	hero->equip(new Ice());
	hero->equip(new Cure());
	hero->equip(new Ice());
	hero->equip(new Cure());\
	AMateria* overflow = new Ice();//doit fail car l inventaire est full (max 4)
	hero->equip(overflow);
	delete overflow;
	std::cout << std::endl;
	
	//tests utilisation des objets equipes
	Character target("Enemy");
	hero->use(0, target);
	hero->use(1, target);
	hero->use(2, target);
	hero->use(3, target);
	hero->use(4, target);//index invalide doit fail
	std::cout << std::endl;
	
	//test unequip et use
	hero->unequip(1);
	hero->use(1, target); // doit fail car on vient d unequip
	hero->equip(new Ice());
	hero->use(1, target);
	hero->unequip(5); // index invalide
	hero->unequip(1);
	hero->unequip(1);
	std::cout << std::endl;
	
	//test deep copy
	Character original("Original");
	original.equip(new Ice());
	original.equip(new Cure());
	Character copy = original;
	std::cout << "Original uses materia:" << std::endl;
	original.use(0, target);
	std::cout << "Copy uses materia:" << std::endl;
	copy.use(0, target);
	std::cout << std::endl;
	
	//test limite de learnmateria
	IMateriaSource* source = new MateriaSource();
	source->learnMateria(new Ice());
	source->learnMateria(new Cure());
	source->learnMateria(new Ice());
	source->learnMateria(new Cure());
	AMateria* overflow2 = new Ice();
	source->learnMateria(overflow2);//doit fail car max materia apprises
	delete overflow2;
	std::cout << std::endl;
	
	delete source;
	delete hero;
}