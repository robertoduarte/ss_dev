#ifndef OBJECT_H
#define OBJECT_H

/*-----------------Object Defines-------------------*/
typedef void* Object;
#define OBJECT_HEADER_CAST(object) (*((ObjectHeader**) object))
#define UNUSED_INTERFACE 0

/*-----------------Draw Interface-------------------*/
typedef void (* draw_interface)(Object);
void i_draw(Object);

/*--------------Interaction Interface---------------*/
typedef void (* interaction_interface)(Object, Object);
void i_interaction(Object, Object);

/*-------------Object Header Definition-------------*/
typedef struct ObjectHeader
{
    int type_id;
    draw_interface i_draw;
    interaction_interface i_interaction;
} ObjectHeader;

/*-----------Object Functions Declarations----------*/
int is_type(Object, int type);

#endif /* OBJECT_H */

