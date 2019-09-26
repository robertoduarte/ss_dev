	slPushMatrix();
	{
		set_polygon(pos_model_0, ang_model_0, scl_model_0);
		slPushMatrix();
		{
			set_polygon(pos_model_1, ang_model_1, scl_model_1);
			slPushMatrix();
			{
				set_polygon(pos_model_2, ang_model_2, scl_model_2);
				slPutPolygon(PD_MODEL_2);
			}
			slPopMatrix();
		}
		slPopMatrix();
		slPushMatrix();
		{
			set_polygon(pos_model_3, ang_model_3, scl_model_3);
			slPushMatrix();
			{
				set_polygon(pos_model_4, ang_model_4, scl_model_4);
				slPutPolygon(PD_MODEL_4);
			}
			slPopMatrix();
		}
		slPopMatrix();
	}
	slPopMatrix();

