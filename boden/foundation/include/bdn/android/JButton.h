#ifndef BDN_ANDROID_JButton_H_
#define BDN_ANDROID_JButton_H_

#include <bdn/android/JTextView.h>
#include <bdn/java/JString.h>

namespace bdn
{
    namespace android
    {

        /** Accessor for Java android.widget.Button objects.*/
        class JButton : public JTextView
        {
          private:
            static bdn::java::Reference newInstance_(JContext &context)
            {
                static bdn::java::MethodId constructorId;

                return getStaticClass_().newInstance_(constructorId, context);
            }

          public:
            JButton(JContext &context) : JTextView(newInstance_(context)) {}

            /** @param javaRef the reference to the Java object.
             *      The JObject instance will copy this reference and keep its
             * type. So if you want the JObject instance to hold a strong
             * reference then you need to call toStrong() on the reference first
             * and pass the result.
             *      */
            explicit JButton(const bdn::java::Reference &javaRef)
                : JTextView(javaRef)
            {}

            JButton() {}

            /** Returns the JClass object for this class.
             *
             *  Note that the returned class object is not necessarily unique
             * for the whole process. You might get different objects if this
             * function is called from different shared libraries.
             *
             *  If you want to check for type equality then you should compare
             * the type name (see getTypeName() )
             *  */
            static bdn::java::JClass &getStaticClass_()
            {
                static bdn::java::JClass cls("android/widget/Button");

                return cls;
            }
        };
    }
}

#endif
