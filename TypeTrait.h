


/*********************************************************************/
//	_____ TypeTrait _____
//	
//	Traits pour les types atomic repris de std::TypeTrait.
//	Fourni les m�thodes n�cessaires pour d�finir la cat�gorie des
//	diff�rents types utilis�s (enum, integral...).
//
//	Seules les m�thodes utiles aux traits des collections sont
//	impl�ment�s.
//
//	On d�fini dans le namespace 'ard_c' tout comme les collections
//	qui vont utilis�s ces traits.
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
	// Struct de base � tout les traits de type.
	// Permet de les stocker sous une expression constante optimis�e.
	template<typename T, T _v>
	struct integral_constant
	{
		static const T value = _v;
		typedef T value_type;
		typedef integral_constant<T, _v> type;
		operator value_type() { return value; }
	};


	// _DEFINE_SPEC
	// Version abr�g�e de la version std. Seul les param�tres n�cessaires
	// � is_integral, is_floating_point et is_enum sont d�fini ici.
	// Permet de sp�cialis� une struct _Trait pour renvoyer true ou false,
	// valeur d�fini par _Val, pour un type _Type donn�.
#ifndef _DEFINE_SPEC
	#define _DEFINE_SPEC(_Trait, _Type, _Val) \
	template<> \
	struct _Trait<_Type> : public integral_constant<bool, _Val> \
	{ };
#endif // !_DEFINE_SPEC


	// false_type
	// Struct de commodit� renvoyant false quoi qu'il arrive.
	typedef integral_constant<bool, false> false_type;


	// __is_integral_helper
	// D�fini quels sont les types integral.
	// L'impl�mentation de base renvoie false.
	template<typename>
	struct __is_integral_helper : public false_type
	{ };
	// On sp�cialise __is_integral_helper pour renvoyer true sur les types integral
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
	// D�fini quels sont les types flottant;
	// L'impl�mentation de base renvoie false.
	template<typename>
	struct __is_floating_point_helper : public false_type
	{ };
	// On sp�cialise __is_floating_point_helper pour renvoyer true les types flottants
	// avec la macro _DEFINE_SPEC
	_DEFINE_SPEC(__is_floating_point_helper, float, true)
	_DEFINE_SPEC(__is_floating_point_helper, double, true)
	_DEFINE_SPEC(__is_floating_point_helper, long double, true)
	

	// remove_volatile
	// Supprime la composante volatile d'un type T pour renvoyer
	// syst�matiquement son type 'non volatile'.
	//
	// Version non volatile -> renvoie le type T fourni � l'identique
	template<typename T>
	struct remove_volatile
	{ typedef T type; };
	// Sp�cialisation volatile -> si le type T fourni est volatile on renvoie T sans la composante volatile
	template<typename T>
	struct remove_volatile<T volatile>
	{ typedef T type; };


	// remove_const
	// Supprime la composante const d'un type T pour renvoyer
	// syst�matiquement son type 'non constant'.
	//
	// Version non constante -> renvoie le type T � l'identique
	template<typename T>
	struct remove_const
	{ typedef T type; };
	// Sp�cialisation const -> si le type T fourni est constant on renvoie T sans la composante const
	template<typename T>
	struct remove_const<T const>
	{ typedef T type; };


	// remove_cv
	// Aggr�gation de remove_volatile et remove_const.
	// Permet de renvoyer un type T sans sa composante 'cv-qualifier'.
	template<typename T>
	struct remove_cv
	{
		typedef typename remove_const<typename remove_volatile<T>::type >::type type;
	};


	// is_integral
	// D�fini si un type T est integral, soit un type bool, char, short, int, long et long long,
	// en prenant en compte les variante unsigned et leur cv-qualifier.
	template<typename T>
	struct is_integral
		: public integral_constant<bool, (__is_integral_helper<typename remove_cv<T>::type>::value)>
	{ };

	// is_floating_point
	// D�fini si un type T est flottant, soit un type float, double et long double,
	// en prenant en compte leur cv-qualifier.
	template<typename T>
	struct is_floating_point 
		: public integral_constant<bool, (__is_floating_point_helper<typename remove_cv<T>::type>::value)>
	{ };


	// is_atomic
	// Aggr�gation de is_integral et is_floating_point. Permet de lister tous les types atomiques.
	template<typename T>
	struct is_atomic
		: public integral_constant<bool, is_integral<T>::value || is_floating_point<T>::value>
	{ };


	// is_enum
	// D�fini si un type T est une �num�ration.
	//   /\    N�cessite la m�thode magique __is_enum du compilateur.
	//  /!!\   Si le compilateur ne poss�de pas la m�thode cette impl�mentation ne fonctionne pas.
	//  ����
	template<typename T>
	struct is_enum : public integral_constant<bool, __is_enum(T)>
	{ };
}

#endif	// TYPETRAIT_H
