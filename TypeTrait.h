


/*********************************************************************/
//	_____ TypeTrait _____
//	
//	Traits pour les types atomic repris de std::TypeTrait.
//	Fourni les méthodes nécessaires pour définir la catégorie des
//	différents types utilisés (enum, integral...).
//
//	Seules les méthodes utiles aux traits des collections sont
//	implémentés.
//
//	On défini dans le namespace 'ard_c' tout comme les collections
//	qui vont utilisés ces traits.
//
//	Source de type_traits.h :
//	https://gcc.gnu.org/onlinedocs/gcc-4.6.2/libstdc++/api/a01090_source.html
//	
/*********************************************************************/



#ifndef TYPETRAIT_H
#define TYPETRAIT_H

namespace ard_c
{

	// integral_constant
	// Struct de base à tout les traits de type.
	// Permet de les stocker sous une expression constante optimisée.
	template<typename T, T _v>
	struct integral_constant
	{
		static const T value = _v;
		typedef T value_type;
		typedef integral_constant<T, _v> type;
		operator value_type() { return value; }
	};


	// _DEFINE_SPEC
	// Version abrégée de la version std. Seul les paramètres nécessaires
	// à is_integral, is_floating_point et is_enum sont défini ici.
	// Permet de spécialisé une struct _Trait pour renvoyer true ou false,
	// valeur défini par _Val, pour un type _Type donné.
#ifndef _DEFINE_SPEC
	#define _DEFINE_SPEC(_Trait, _Type, _Val) \
	template<> \
	struct _Trait<_Type> : public integral_constant<bool, _Val> \
	{ };
#endif // !_DEFINE_SPEC


	// false_type
	// Struct de commodité renvoyant false quoi qu'il arrive.
	typedef integral_constant<bool, false> false_type;


	// __is_integral_helper
	// Défini quels sont les types integral.
	// L'implémentation de base renvoie false.
	template<typename>
	struct __is_integral_helper : public false_type
	{ };
	// On spécialise __is_integral_helper pour renvoyer true sur les types integral
	// avec la macro _DEFINE_SPEC
	_DEFINE_SPEC(__is_integral_helper, bool, true)
	_DEFINE_SPEC(__is_integral_helper, char, true)
	_DEFINE_SPEC(__is_integral_helper, signed char, true)
	_DEFINE_SPEC(__is_integral_helper, unsigned char, true)
	_DEFINE_SPEC(__is_integral_helper, short, true)
	_DEFINE_SPEC(__is_integral_helper, unsigned short, true)
	_DEFINE_SPEC(__is_integral_helper, int, true)
	_DEFINE_SPEC(__is_integral_helper, unsigned int, true)
	_DEFINE_SPEC(__is_integral_helper, long, true)
	_DEFINE_SPEC(__is_integral_helper, unsigned long, true)
	_DEFINE_SPEC(__is_integral_helper, long long, true)
	_DEFINE_SPEC(__is_integral_helper, unsigned long long, true)


	// __is_floating_point_helper
	// Défini quels sont les types flottant;
	// L'implémentation de base renvoie false.
	template<typename>
	struct __is_floating_point_helper : public false_type
	{ };
	// On spécialise __is_floating_point_helper pour renvoyer true les types flottants
	// avec la macro _DEFINE_SPEC
	_DEFINE_SPEC(__is_floating_point_helper, float, true)
	_DEFINE_SPEC(__is_floating_point_helper, double, true)
	_DEFINE_SPEC(__is_floating_point_helper, long double, true)
	

	// remove_volatile
	// Supprime la composante volatile d'un type T pour renvoyer
	// systématiquement son type 'non volatile'.
	//
	// Version non volatile -> renvoie le type T fourni à l'identique
	template<typename T>
	struct remove_volatile
	{ typedef T type; };
	// Spécialisation volatile -> si le type T fourni est volatile on renvoie T sans la composante volatile
	template<typename T>
	struct remove_volatile<T volatile>
	{ typedef T type; };


	// remove_const
	// Supprime la composante const d'un type T pour renvoyer
	// systématiquement son type 'non constant'.
	//
	// Version non constante -> renvoie le type T à l'identique
	template<typename T>
	struct remove_const
	{ typedef T type; };
	// Spécialisation const -> si le type T fourni est constant on renvoie T sans la composante const
	template<typename T>
	struct remove_const<T const>
	{ typedef T type; };


	// remove_cv
	// Aggrégation de remove_volatile et remove_const.
	// Permet de renvoyer un type T sans sa composante 'cv-qualifier'.
	template<typename T>
	struct remove_cv
	{
		typedef typename remove_const<typename remove_volatile<T>::type >::type type;
	};


	// is_integral
	// Défini si un type T est integral, soit un type bool, char, short, int, long et long long,
	// en prenant en compte les variante unsigned et leur cv-qualifier.
	template<typename T>
	struct is_integral
		: public integral_constant<bool, (__is_integral_helper<typename remove_cv<T>::type>::value)>
	{ };

	// is_floating_point
	// Défini si un type T est flottant, soit un type float, double et long double,
	// en prenant en compte leur cv-qualifier.
	template<typename T>
	struct is_floating_point 
		: public integral_constant<bool, (__is_floating_point_helper<typename remove_cv<T>::type>::value)>
	{ };


	// is_atomic
	// Aggrégation de is_integral et is_floating_point. Permet de lister tous les types atomiques.
	template<typename T>
	struct is_atomic
		: public integral_constant<bool, is_integral<T>::value || is_floating_point<T>::value>
	{ };


	// is_enum
	// Défini si un type T est une énumération.
	//   /\    Nécessite la méthode magique __is_enum du compilateur.
	//  /!!\   Si le compilateur ne posséde pas la méthode cette implémentation ne fonctionne pas.
	//  ¯¯¯¯
	template<typename T>
	struct is_enum : public integral_constant<bool, __is_enum(T)>
	{ };
}

#endif	// TYPETRAIT_H
