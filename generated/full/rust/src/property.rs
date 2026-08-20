#[allow(unused_imports)]
use crate::payloads::*;
use derive_builder::Builder;

#[derive(Clone, Builder, Debug)]
pub struct FullInitialPropertyValues {
    pub favorite_number: i32,
    pub favorite_number_version: u32,

    pub favorite_foods: FavoriteFoodsProperty,
    pub favorite_foods_version: u32,

    pub lunch_menu: LunchMenuProperty,
    pub lunch_menu_version: u32,

    pub family_name: String,
    pub family_name_version: u32,

    pub last_breakfast_time: chrono::DateTime<chrono::Utc>,
    pub last_breakfast_time_version: u32,

    pub last_birthdays: LastBirthdaysProperty,
    pub last_birthdays_version: u32,
}

#[allow(dead_code)]
impl FullInitialPropertyValues {
    /// Validate the initial values of properties that declare a JSON schema constraint.
    ///
    /// Returns `Ok(())` if all schema-constrained properties conform to their schema,
    /// or if no property declares a schema.  Returns `Err` with a description if a
    /// constrained property's value does not conform.
    pub fn validate_schema(&self) -> Result<(), String> {
        Ok(())
    }
}
