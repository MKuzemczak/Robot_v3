#ifndef LISTA_H
#define LISTA_H


#include <list>
#include <QDebug>

template<typename T>
    class Lista : public std::list<T>
    {
    public:
        T & operator [](const int index)
        {
            if(index < 0 || index > static_cast<int>(this->size()) - 1)
            {
                qDebug() << "Lista::operator [](const int) : index out of bounds, index == " << index;
            }

            int i = 0;
            for (T & r : (*this))
            {
                if (i == index)
                    return r;
                i++;
            }

            return this->front();
        }

        void erase(int index)
        {
            int i = 0;

            for (auto it = std::list<T>::begin(); it != std::list<T>::end();)
            {
                if (i++ == index)
                {
                    it = std::list<T>::erase(it);
                    break;
                }
                else
                    ++it;
            }

        }

        auto iteratorAt(int i)
        {
            if(i < 0 || i > static_cast<int>(this->size()) - 1)
            {
                qDebug() << "Lista::iteratorAt(int) : index out of bounds";
            }

            int j = 0;
            for (auto it = (*this).begin(); j <= i; it++)
            {
                if (j == i)
                    return it;

                j++;
            }

            return this->end();
        }
    };

#endif // LISTA_H
